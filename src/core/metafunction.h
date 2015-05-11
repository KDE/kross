/***************************************************************************
 * metafunction.h
 * This file is part of the KDE project
 * copyright (C)2005-2006 Ian Reinhart Geiser <geiseri@kde.org>
 * copyright (C)2005-2006 Matt Broadstone     <mbroadst@gmail.com>
 * copyright (C)2005-2006 Richard J. Moore    <rich@kde.org>
 * copyright (C)2005-2006 Erik L. Bunce       <kde@bunce.us>
 * copyright (C)2005-2007 by Sebastian Sauer  <mail@dipe.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef KROSS_METAFUNCTION_H
#define KROSS_METAFUNCTION_H

#include <krosscore_export.h>
#include <QtCore/QObject>
#include <QtCore/QArgument>
#include <QtCore/QByteRef>
#include <QtCore/QPointer>
#include <QtCore/QVector>

namespace Kross
{

static const char s_fakeClassName[] = "ScriptFunction";

/**
 * The MetaFunction class implements a QObject to provide an adaptor
 * between Qt signals+slots and scripting functions.
 *
 * For example the Kross::PythonFunction and the Kross::RubyFunction
 * classes located in kdebindings inherit this class to connect a
 * QObject signal together with a callable python or ruby method.
 */
class KROSSCORE_EXPORT MetaFunction : public QObject
{
public:

    /**
    * Constructor.
    *
    * \param sender The QObject instance that sends the signal.
    * \param signal The signature of the signal the QObject emits.
    * \param callable The callable python function that should
    * be executed if the QObject emits the signal.
    */
    MetaFunction(QObject *sender, const QByteArray &signal)
        : QObject(), m_sender(sender), m_signature(QMetaObject::normalizedSignature(signal.constData()))
    {
        //krossdebug(QString("MetaFunction sender=\"%1\" signal=\"%2\"").arg(sender->objectName()).arg(m_signature.constData()));

        const QByteArray signalName = signal.left(signal.indexOf('('));

        const QList<QByteArray> types = parameterTypeNamesFromSignature(m_signature.constData());

        QList<int> parameterMetaTypes;
        parameterMetaTypes.append(QMetaType::Void); // return type
        foreach(const QByteArray &typeName, types) {
            parameterMetaTypes.append(QMetaType::type(typeName.constData())); // ## might be missing support for non-builtin types...
        }

        m_data.resize(20 + parameterMetaTypes.count());

        // content
        m_data[0] = 7;  // revision
        m_data[1] = 0;  // classname (the first string)
        m_data[2] = 0;  // classinfo count
        m_data[3] = 0;  // classinfo data
        m_data[4] = 1;  // methods count
        m_data[5] = 14; // methods data
        m_data[6] = 0;  // properties count
        m_data[7] = 0;  // properties data
        m_data[8] = 0;  // enums/sets count
        m_data[9] = 0;  // enums/sets data
        m_data[10] = 0; // constructors count
        m_data[11] = 0; // constructors data
        m_data[12] = 0; // flags
        m_data[13] = 0; // signal count

        // slots: name, argc, parameters, tag, flags
        m_data[14] = 1;  // name
        m_data[15] = types.count();  // parameter count
        m_data[16] = 19;  // parameter data
        m_data[17] = 2;  // tag (RequiresVariantMetaObject)
        m_data[18] = 0x0a; // flags (public slot)

        // slots: parameters
        int i = 19;
        foreach (int metaType, parameterMetaTypes) {
            m_data[i++] = metaType;
        }

        m_data[i++] = 0;    // eod

        // string table
        // qt_metacast expects the first string in the string table to be the class name.
        const QByteArray className(s_fakeClassName);
        int offsetOfStringdataMember = 2 * sizeof(QByteArrayData);
        int stringdataOffset = 0;
        m_stringData = new char[offsetOfStringdataMember + className.size() + 1 + signalName.size() + 1];
        writeString(m_stringData, /*index*/0, className, offsetOfStringdataMember, stringdataOffset);
        writeString(m_stringData, 1, signalName, offsetOfStringdataMember, stringdataOffset);

        // static metaobject
        staticMetaObject.d.superdata = &QObject::staticMetaObject;
        staticMetaObject.d.stringdata = reinterpret_cast<const QByteArrayData *>(m_stringData);
        staticMetaObject.d.data = m_data.data();
        staticMetaObject.d.relatedMetaObjects = 0;
        staticMetaObject.d.extradata = 0;
        staticMetaObject.d.static_metacall = 0;
    }

    /**
    * Destructor.
    */
    virtual ~MetaFunction() {
        delete[] m_stringData;
    }

    /**
    * The static \a QMetaObject instance that provides the
    * QMeta-information for this QObject class.
    */
    QMetaObject staticMetaObject;

    /**
    * \return the dynamic build \a QMetaObject instance
    * for this QObject instance.
    */
    const QMetaObject *metaObject() const Q_DECL_OVERRIDE
    {
        return &staticMetaObject;
    }

    /**
    * Try to cast this QObject instance into the class with
    * name \p _clname and return the casted pointer or NULL
    * if casting failed.
    */
    void *qt_metacast(const char *_clname) Q_DECL_OVERRIDE
    {
        if (! _clname) {
            return 0;
        }
        if (! qstrcmp(_clname, s_fakeClassName)) {
            return static_cast<void *>(const_cast< MetaFunction * >(this));
        }
        return QObject::qt_metacast(_clname);
    }

    /**
    * This method got called if a method this QObject instance
    * defines should be invoked.
    */
    int qt_metacall(QMetaObject::Call _c, int _id, void **_a) = 0;

protected:
    /// The sender QObject.
    QPointer<QObject> m_sender;
    /// The signature.
    QByteArray m_signature;

private:
    /// The stringdata.
    char* m_stringData;
    /// The data array.
    QVector<uint> m_data;

    // from Qt5's qmetaobjectbuilder.cpp
    static void writeString(char *out, int i, const QByteArray &str,
            const int offsetOfStringdataMember, int &stringdataOffset)
    {
        int size = str.size();
        qptrdiff offset = offsetOfStringdataMember + stringdataOffset
            - i * sizeof(QByteArrayData);
        const QByteArrayData data =
            Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(size, offset);
        memcpy(out + i * sizeof(QByteArrayData), &data, sizeof(QByteArrayData));
        memcpy(out + offsetOfStringdataMember + stringdataOffset, str.constData(), size);
        out[offsetOfStringdataMember + stringdataOffset + size] = '\0';
        stringdataOffset += size + 1;
    }

    // from Qt5's QMetaObjectPrivate
    QList<QByteArray> parameterTypeNamesFromSignature(const char *signature)
    {
        QList<QByteArray> list;
        while (*signature && *signature != '(')
            ++signature;
        while (*signature && *signature != ')' && *++signature != ')') {
            const char *begin = signature;
            int level = 0;
            while (*signature && (level > 0 || *signature != ',') && *signature != ')') {
                if (*signature == '<')
                    ++level;
                else if (*signature == '>')
                    --level;
                ++signature;
            }
            list += QByteArray(begin, signature - begin);
        }
        return list;
    }
};

}

#endif
