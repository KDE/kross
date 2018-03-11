/***************************************************************************
 * metafunction.cpp
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

#include "metafunction.h"

#include <QVector>

namespace Kross
{

static const char s_fakeClassName[] = "ScriptFunction";

class KROSSCORE_NO_EXPORT MetaFunction::Private
{
public:
    /// The stringdata.
    char* stringData;
    /// The data array.
    QVector<uint> data;
};

MetaFunction::MetaFunction(QObject *sender, const QByteArray &signal)
    : QObject()
    , m_sender(sender)
    , m_signature(QMetaObject::normalizedSignature(signal.constData()))
    , d(new Private)
{
    //krossdebug(QString("MetaFunction sender=\"%1\" signal=\"%2\"").arg(sender->objectName()).arg(m_signature.constData()));

    const QByteArray signalName = signal.left(signal.indexOf('('));

    const QList<QByteArray> types = parameterTypeNamesFromSignature(m_signature.constData());

    QList<int> parameterMetaTypes;
    parameterMetaTypes.append(QMetaType::Void); // return type
    foreach(const QByteArray &typeName, types) {
        parameterMetaTypes.append(QMetaType::type(typeName.constData())); // ## might be missing support for non-builtin types...
    }

    QVector<uint>& dataRef = d->data;
    dataRef.resize(20 + parameterMetaTypes.count());

    // content
    dataRef[0] = 7;  // revision
    dataRef[1] = 0;  // classname (the first string)
    dataRef[2] = 0;  // classinfo count
    dataRef[3] = 0;  // classinfo data
    dataRef[4] = 1;  // methods count
    dataRef[5] = 14; // methods data
    dataRef[6] = 0;  // properties count
    dataRef[7] = 0;  // properties data
    dataRef[8] = 0;  // enums/sets count
    dataRef[9] = 0;  // enums/sets data
    dataRef[10] = 0; // constructors count
    dataRef[11] = 0; // constructors data
    dataRef[12] = 0; // flags
    dataRef[13] = 0; // signal count

    // slots: name, argc, parameters, tag, flags
    dataRef[14] = 1;  // name
    dataRef[15] = types.count();  // parameter count
    dataRef[16] = 19;  // parameter data
    dataRef[17] = 2;  // tag (RequiresVariantMetaObject)
    dataRef[18] = 0x0a; // flags (public slot)

    // slots: parameters
    int i = 19;
    foreach (int metaType, parameterMetaTypes) {
        dataRef[i++] = metaType;
    }

    dataRef[i++] = 0;    // eod

    // string table
    // qt_metacast expects the first string in the string table to be the class name.
    const QByteArray className(s_fakeClassName);
    int offsetOfStringdataMember = 2 * sizeof(QByteArrayData);
    int stringdataOffset = 0;
    d->stringData = new char[offsetOfStringdataMember + className.size() + 1 + signalName.size() + 1];
    writeString(d->stringData, /*index*/0, className, offsetOfStringdataMember, stringdataOffset);
    writeString(d->stringData, 1, signalName, offsetOfStringdataMember, stringdataOffset);

    // static metaobject
    staticMetaObject.d.superdata = &QObject::staticMetaObject;
    staticMetaObject.d.stringdata = reinterpret_cast<const QByteArrayData *>(d->stringData);
    staticMetaObject.d.data = dataRef.data();
    staticMetaObject.d.relatedMetaObjects = nullptr;
    staticMetaObject.d.extradata = nullptr;
    staticMetaObject.d.static_metacall = nullptr;
}

MetaFunction::~MetaFunction() {
    delete[] d->stringData;
    delete d;
}

const QMetaObject *MetaFunction::metaObject() const
{
    return &staticMetaObject;
}

void *MetaFunction::qt_metacast(const char *_clname)
{
    if (! _clname) {
        return nullptr;
    }
    if (! qstrcmp(_clname, s_fakeClassName)) {
        return static_cast<void *>(const_cast< MetaFunction * >(this));
    }
    return QObject::qt_metacast(_clname);
}

// from Qt5's qmetaobjectbuilder.cpp
void MetaFunction::writeString(char *out, int i, const QByteArray &str,
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
QList<QByteArray> MetaFunction::parameterTypeNamesFromSignature(const char *signature)
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

}
