/*
 * Copyright (C) 2018 lixiang
 *
 * Authors:
 *  lixiang kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "myworker.h"
#include "utils.h"

#include <QDebug>
#include <QTime>
#include <QDBusReply>

inline const QString covertKeyName(const QString &key)
{
    if(key == "Computer_Vendor")
        return QString(QObject::tr("Vendor"));
    else if(key == "Computer_Product")
        return QString(QObject::tr("Model"));
    else if(key == "Computer_Version")
        return QString(QObject::tr("Version"));
    else if(key == "CPU_Capacity")
        return QString(QObject::tr("Maximum Frequency"));
    else if(key == "Computer_Serial")
        return QString(QObject::tr("Serial Number"));

    else if(key == "CPU_Version")
        return QString(QObject::tr("CPU"));
    else if(key == "CPU_Cores")
        return QString(QObject::tr("Core Number"));
    else if(key == "CPU_Vendor")
        return QString(QObject::tr("Vendor"));
    else if(key == "CPU_Serial")
        return QString(QObject::tr("Serial Number"));
    else if(key == "CPU_Capacity")
        return QString(QObject::tr("Maximum Frequency"));
    else if(key == "CPU_Size")
        return QString(QObject::tr("Current Frequency"));

    else if(key == "Memory_Count")
        return QString(QObject::tr("Count"));
    else if(key == "Memory_Slot")
        return QString(QObject::tr("Slot Number"));
    else if(key == "Memory_Product")
        return QString(QObject::tr("Memory Model"));
    else if(key == "Memory_Vendor")
        return QString(QObject::tr("Vendor"));
    else if(key == "Memory_Serial")
        return QString(QObject::tr("Serial Number"));
    else if(key == "Memory_Size")
        return QString(QObject::tr("Memory Size"));
    else if(key == "Memory_Width")
        return QString(QObject::tr("Data Width"));

    else
        return key;
}

MyWorker::MyWorker(QObject *parent)
    : QObject(parent)
{
    m_dbusInterface = new QDBusInterface("org.freedesktop.systemtool",
                               "/",
                               "",
                               QDBusConnection::systemBus());
}

MyWorker::~MyWorker()
{
    this->quitDbusServer();

    if (m_dbusInterface ) {
        delete m_dbusInterface;
        m_dbusInterface = nullptr;
    }
}

void MyWorker::quitDbusServer()
{
    m_dbusInterface->call("exitDbus");
}

void MyWorker::refreshSystemInfo()
{
    QMap<QString, QVariant> result;
    QDBusReply<QMap<QString, QVariant> > reply = m_dbusInterface->call("getSystemInfo");
    if (reply.isValid()) {
        result = reply.value();
        if (!result.isEmpty()) {
            QMap<QString, QString> computer_map;
            QMap<QString, QString> cpu_map;
            QMap<QString, QString> memory_map;
            QMap<QString, QVariant>::iterator it;
            for (it = result.begin(); it != result.end(); ++it) {
                if (it.key().startsWith("Computer_") && it.value().toString().length() > 0) {
                    computer_map.insert(covertKeyName(it.key()), it.value().toString());
                }
                else if (it.key().startsWith("CPU_") && it.value().toString().length() > 0) {
                    cpu_map.insert(covertKeyName(it.key()), it.value().toString());
                }
                else if (it.key().startsWith("Memory_") && it.value().toString().length() > 0) {
                    QString mem_Info = it.value().toString();
                    if (mem_Info.contains('$'))
                        mem_Info.replace("$", " ------  ");
                    memory_map.insert(covertKeyName(it.key()), mem_Info);
                }
            }
            if(!computer_map.isEmpty() && computer_map.count() > 0) {
                emit this->sendInformation(SystemInfoID::COMPUTER_INFO, computer_map);
            }
            if(!cpu_map.isEmpty() && cpu_map.count() > 0) {
                emit this->sendInformation(SystemInfoID::CPU_INFO, cpu_map);
            }
            if(!memory_map.isEmpty() && memory_map.count() > 0) {
                emit this->sendInformation(SystemInfoID::MEMORY_INFO, memory_map);
            }
            computer_map.clear();
            cpu_map.clear();
            memory_map.clear();
        }
    }
    else {
        qDebug() << "call getSystemInfo failed!";
    }

    //test data
    /*qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int a = qrand()%9;

    QMap<QString, QString> value;
    value.insert("Computer_Vendor", QString::number(a));
    value.insert("Computer_Product", "bb");
    value.insert("Computer_Version", "cc");
    value.insert("Computer_Serial", "dd");
    emit this->sendInformation(0, value);

    QMap<QString, QString> value1;
    value1.insert("CPU_Vendor", QString::number(a));
    value1.insert("CPU_Cores", "ff");
    value1.insert("CPU_Version", "gg");
    value1.insert("CPU_Capacity", "hh");
    value1.insert("CPU_Size", "jj");
    value1.insert("CPU_Serial", "ii");
    emit this->sendInformation(1, value1);

    QMap<QString, QString> value2;
    value2.insert("Memory_Count", "1");
    value2.insert("Memory_Vendor", QString::number(a));
    value2.insert("Memory_Product", "ll");
    value2.insert("Memory_Size", "mm");
    value2.insert("Memory_Serial", "nn");
    value2.insert("Memory_Slot", "pp");
    value2.insert("Memory_Width", "qq");
    emit this->sendInformation(2, value2);*/
}
