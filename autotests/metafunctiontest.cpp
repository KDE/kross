/* This file is part of the KDE frameworks
    Copyright (c) 2015 David Faure <faure@kde.org>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License or ( at
    your option ) version 3 or, at the discretion of KDE e.V. ( which shall
    act as a proxy as in section 14 of the GPLv3 ), any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QDebug>
#include <kross/core/metafunction.h>
#include <QMetaMethod>
#include <QTest>

class MyFunction : public Kross::MetaFunction
{
public:
    MyFunction(QObject* sender, const QByteArray& signal)
        : Kross::MetaFunction(sender, signal) {}

    int qt_metacall(QMetaObject::Call _c, int _id, void **_a) override {
        _id = QObject::qt_metacall(_c, _id, _a);
        if (_id >= 0 && _c == QMetaObject::InvokeMetaMethod) {
            switch (_id) {
                case 0:
                {
                    // convert the arguments
                    QMetaMethod method = metaObject()->method( metaObject()->indexOfMethod(m_signature.constData()) );
                    m_paramTypes = method.parameterTypes();
                    for (int idx = 0; idx < m_paramTypes.size(); ++idx) {
                        const int tp = QVariant::nameToType( m_paramTypes.at(idx).constData() );
                        QVariant v(tp, _a[idx+1]);
                        m_paramValues.append(v);
                    }
                }
            }
            _id -= 1;
        }
        return _id;
    }
    QList<QByteArray> paramTypes() const { return m_paramTypes; }
    QList<QVariant> paramValues() const { return m_paramValues; }
private:
    QList<QByteArray> m_paramTypes;
    QList<QVariant> m_paramValues;
};

class WrappedObject : public QObject
{
    Q_OBJECT
public:
    void doEmit() { emit valueChanged(42); }
Q_SIGNALS:
    void valueChanged(int);
};

class MetaFunctionTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testMetaFunction()
    {
        WrappedObject obj;
        QList<MyFunction *> funcs;
        // mostly from kross-interpreters/python/pythonscript.cpp
        const QMetaObject* metaobject = obj.metaObject();
        const int count = metaobject->methodCount();
        for(int i = 0; i < count; ++i) {
            QMetaMethod metamethod = metaobject->method(i);
            if( metamethod.methodType() == QMetaMethod::Signal ) {
                const QByteArray signature = metamethod.methodSignature();
                // skip signals from QObject
                if (signature.startsWith("destroyed") || signature.startsWith("objectNameChanged"))
                    continue;
                MyFunction *func = new MyFunction(&obj, signature);
                funcs.append(func);
                const QByteArray name = signature.left(signature.indexOf('('));
                QByteArray sendersignal = signature;
                sendersignal.prepend(QSIGNAL_CODE + '0');
                QByteArray receiverslot = signature;
                receiverslot.prepend(QSLOT_CODE + '0');

                QObject::connect(&obj, sendersignal.constData(), func, receiverslot.constData());
            }
        }

        QCOMPARE(funcs.count(), 1); // valueChanged

        MyFunction* valueChangedFunc = funcs.at(0);
        const QMetaObject *valueChangedMetaObj = valueChangedFunc->metaObject();
        bool found = false;
        QByteArray foundSig;
        for (int i = 0; i < valueChangedMetaObj->methodCount(); ++i) {
            QMetaMethod method = valueChangedMetaObj->method(i);
            if (method.name() == "valueChanged") {
                found = true;
                foundSig = method.methodSignature();
            }
        }
        QVERIFY2(found, "no valueChanged method/slot found in dynamic meta object!");
        QCOMPARE(foundSig.constData(), "valueChanged(int)");

        obj.doEmit();
        QCOMPARE(funcs.at(0)->paramTypes(), QList<QByteArray>() << "int");
        QCOMPARE(funcs.at(0)->paramValues().at(0).toInt(), 42);
        qDeleteAll(funcs);
    }
};

QTEST_MAIN(MetaFunctionTest)

#include "metafunctiontest.moc"
