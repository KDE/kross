/***************************************************************************
 * metatype.h
 * This file is part of the KDE project
 * copyright (C)2004-2006 by Sebastian Sauer (mail@dipe.org)
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

#ifndef KROSS_METATYPE_H
#define KROSS_METATYPE_H

#include "krossconfig.h"
//#include "object.h"

#include <QStringList>
#include <QVariant>
#include <QMetaType>

#include <typeinfo>

//#include <QDate>
//#include <QTime>
//#include <QDateTime>

namespace Kross
{

/**
 * Base class for metatype-implementations.
 */
class MetaType
{
public:
    virtual ~MetaType() {}

    virtual int typeId() = 0;
    //virtual QObject* toObject() = 0;
    //virtual QVariant toVariant() = 0;
    virtual void *toVoidStar() = 0;
};

/**
 * Metatypes which are registered in the QMetaType system.
 */
template<typename METATYPE>
class MetaTypeImpl : public MetaType
{
public:
    MetaTypeImpl(const METATYPE &v) : m_variant(v)
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeImpl<METATYPE> Ctor typeid=%1 typename=%2").arg(qMetaTypeId<METATYPE>()).arg(typeid(METATYPE).name()));
#endif
    }
    ~MetaTypeImpl() override
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeImpl<METATYPE> Dtor typeid=%1 typename=%2").arg(qMetaTypeId<METATYPE>()).arg(typeid(METATYPE).name()));
#endif
    }

    int typeId() override
    {
        return qMetaTypeId<METATYPE>();
    }
    //virtual QVariant toVariant() { return QVariant(typeId(), m_variant); }
    void *toVoidStar() override
    {
        return static_cast<void *>(&m_variant);
    }

private:
    METATYPE m_variant;
};

/**
 * Metatypes which are listened in QVariant::Type.
 */
template<typename VARIANTTYPE>
class MetaTypeVariant : public MetaType
{
public:
    MetaTypeVariant(const VARIANTTYPE &v) : m_value(v)
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeVariant<VARIANTTYPE> Ctor value=%1 typename=%2").arg(QVariant::fromValue(m_value).toString()).arg(QVariant::fromValue(m_value).typeName()));
#endif
    }
    ~MetaTypeVariant() override
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeVariant<VARIANTTYPE> Dtor value=%1 typename=%2").arg(QVariant::fromValue(m_value).toString()).arg(QVariant::fromValue(m_value).typeName()));
#endif
    }

    int typeId() override
    {
        return QVariant::fromValue(m_value).type();
    }
    //virtual QVariant toVariant() { return QVariant::fromValue(m_value); }
    void *toVoidStar() override
    {
        return static_cast<void *>(&m_value);
    }

private:
    VARIANTTYPE m_value;
};

template<>
class MetaTypeVariant<QVariant> : public MetaType
{
public:
    MetaTypeVariant(const QVariant &v) : m_value(v)
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeVariant<QVariant> Ctor value=%1 typename=%2").arg(QVariant::fromValue(m_value).toString()).arg(QVariant::fromValue(m_value).typeName()));
#endif
    }

    ~MetaTypeVariant() override
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeVariant<QVariant> Dtor value=%1 typename=%2").arg(QVariant::fromValue(m_value).toString()).arg(QVariant::fromValue(m_value).typeName()));
#endif
    }

    int typeId() override
    {
        return QVariant::fromValue(m_value).type();
    }

    void *toVoidStar() override
    {
        return m_value.data();
    }

private:
    QVariant m_value;
};

/**
 * Metatype for generic VoidStar pointers.
 */
class MetaTypeVoidStar : public MetaType
{
public:
    MetaTypeVoidStar(int typeId, void *ptr, bool owner) : m_typeId(typeId), m_ptr(ptr), m_owner(owner)
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeVoidStar Ctor typeid=%1 typename=%2 owner=%3").arg(m_typeId).arg(typeid(m_ptr).name()).arg(m_owner));
#endif
    }
    ~MetaTypeVoidStar() override
    {
#ifdef KROSS_METATYPE_DEBUG
        krossdebug(QString("MetaTypeVoidStar Ctor typeid=%1 typename=%2 owner=%3").arg(m_typeId).arg(typeid(m_ptr).name()).arg(m_owner));
#endif
        if (m_owner) {
            QMetaType::destroy(m_typeId, m_ptr);
        }
    }
    int typeId() override
    {
        return m_typeId;
    }
    void *toVoidStar() override
    {
        return static_cast<void *>(&m_ptr);
    }

private:
    int m_typeId;
    void *m_ptr;
    bool m_owner;
};

/**
 * Base class for metatype-handlers as used returned by
 * the Kross::Manager::metaTypeHandler() method.
 *
 * \since 4.2
 */
class KROSSCORE_EXPORT MetaTypeHandler
{
public:
    typedef QVariant(FunctionPtr)(void *);
    typedef QVariant(FunctionPtr2)(MetaTypeHandler *handler, void *);

    explicit MetaTypeHandler() : m_func1(nullptr), m_func2(nullptr) {}
    explicit MetaTypeHandler(FunctionPtr *func) : m_func1(func), m_func2(nullptr) {}
    explicit MetaTypeHandler(FunctionPtr2 *func) : m_func1(nullptr), m_func2(func) {}
    virtual ~MetaTypeHandler() {}

    /**
     * This got called by the scripting-backend if the type-handler
     * is called to translate a void-star pointer to a QVariant.
     */
    virtual QVariant callHandler(void *ptr)
    {
        return m_func1 ? m_func1(ptr) : m_func2 ? m_func2(this, ptr) : QVariant();
    }

private:
    FunctionPtr  *m_func1;
    FunctionPtr2 *m_func2;
};
}

#endif
