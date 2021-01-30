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

#include <kross/core/krosscore_export.h>
#include <QObject>
#include <QPointer>

namespace Kross
{

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
    */
    MetaFunction(QObject *sender, const QByteArray &signal);

    /**
    * Destructor.
    */
    ~MetaFunction() override;

    /**
    * The static \a QMetaObject instance that provides the
    * QMeta-information for this QObject class.
    */
    QMetaObject staticMetaObject;

    /**
    * \return the dynamic build \a QMetaObject instance
    * for this QObject instance.
    */
    const QMetaObject *metaObject() const override;

    /**
    * Try to cast this QObject instance into the class with
    * name \p _clname and return the casted pointer or NULL
    * if casting failed.
    */
    void *qt_metacast(const char *_clname) override;

    /**
    * This method got called if a method this QObject instance
    * defines should be invoked.
    */
    int qt_metacall(QMetaObject::Call _c, int _id, void **_a) override = 0;

protected:
    /// The sender QObject.
    QPointer<QObject> m_sender;
    /// The signature.
    QByteArray m_signature;

private:
    class Private;
    Private *const d;

    static void writeString(
        char *out, int i, const QByteArray &str,
        const int offsetOfStringdataMember, int &stringdataOffset);
    static QList<QByteArray> parameterTypeNamesFromSignature(const char *signature);
};

}

#endif
