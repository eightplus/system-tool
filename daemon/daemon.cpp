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

#include "daemon.h"
#include "utils.h"

#include <QCoreApplication>
#include <QDebug>

Daemon::Daemon()
{

}

void Daemon::exitDbus()
{
    /*QString str;
    str = QString("Dbus Deamon ready to exit!!!");
    emit this->stateChange(str);*/

    qApp->exit(0);
//    exit(0);
}

QMap<QString, QVariant> Daemon::getSystemInfo()
{
    QMap<QString, QVariant> value;

    char manufacturer[128] = "None";
    char productName[128] = "None";
    char version[128] = "None";
    char serial[128] = "None";

    char cpu_manufacturer[128] = "None";
    char cpu_corecount[128] = "None";
    char cpu_version[128] = "None";
    char cpu_maxspeed[128] = "None";
    char cpu_currentspeed[128] = "None";
    char cpu_serial[128] = "None";

    int mem_count = 0;
    char mem_dataWidth[1024] = {0};
    char mem_size[1024] = {0};
    char mem_bankLocator[1024] = {0};
    char mem_type[1024] = {0};
    char mem_manufacturer[1024] = {0};
    char mem_serial[1024] = {0};

    get_computer_info(manufacturer, productName, version, serial);
    get_cpu_info(cpu_manufacturer, cpu_corecount, cpu_version, cpu_maxspeed, cpu_currentspeed, cpu_serial);
    get_memory_info(&mem_count, mem_dataWidth, mem_size, mem_bankLocator, mem_type, mem_manufacturer, mem_serial);

    value.insert("Computer_Vendor", QString::fromStdString(std::string(manufacturer)));
    value.insert("Computer_Product", QString::fromStdString(std::string(productName)));
    value.insert("Computer_Version", QString::fromStdString(std::string(version)));
    value.insert("Computer_Serial", QString::fromStdString(std::string(serial)));

    value.insert("CPU_Vendor", QString::fromStdString(std::string(cpu_manufacturer)));
    value.insert("CPU_Cores", QString::fromStdString(std::string(cpu_corecount)));
    value.insert("CPU_Version", QString::fromStdString(std::string(cpu_version)));
    value.insert("CPU_Capacity", QString::fromStdString(std::string(cpu_maxspeed)));
    value.insert("CPU_Size", QString::fromStdString(std::string(cpu_currentspeed)));
    value.insert("CPU_Serial", QString::fromStdString(std::string(cpu_serial)));

    value.insert("Memory_Count", QString::number(mem_count));
    value.insert("Memory_Vendor", QString::fromStdString(std::string(mem_type)));
    value.insert("Memory_Product", QString::fromStdString(std::string(mem_manufacturer)));
    value.insert("Memory_Size", QString::fromStdString(std::string(mem_size)));
    value.insert("Memory_Serial", QString::fromStdString(std::string(mem_serial)));
    value.insert("Memory_Slot", QString::fromStdString(std::string(mem_bankLocator)));
    value.insert("Memory_Width", QString::fromStdString(std::string(mem_dataWidth)));

//    qDebug() << "manufacturer=" << QString::fromStdString(std::string(manufacturer));
//    qDebug() << "productName=" << QString::fromStdString(std::string(productName));
//    qDebug() << "version=" << QString::fromStdString(std::string(version));
//    qDebug() << "serial=" << QString::fromStdString(std::string(serial));

//    qDebug() << "cpu_manufacturer=" << QString::fromStdString(std::string(cpu_manufacturer));
//    qDebug() << "cpu_corecount=" << QString::fromStdString(std::string(cpu_corecount));
//    qDebug() << "cpu_version=" << QString::fromStdString(std::string(cpu_version));
//    qDebug() << "cpu_maxspeed=" << QString::fromStdString(std::string(cpu_maxspeed));
//    qDebug() << "cpu_currentspeed=" << QString::fromStdString(std::string(cpu_currentspeed));
//    qDebug() << "cpu_serial=" << QString::fromStdString(std::string(cpu_serial));

//    qDebug() << "mem_count=" << mem_count;
//    qDebug() << "mem_dataWidth=" << QString::fromStdString(std::string(mem_dataWidth));
//    qDebug() << "mem_size=" << QString::fromStdString(std::string(mem_size));
//    qDebug() << "mem_bankLocator=" << QString::fromStdString(std::string(mem_bankLocator));
//    qDebug() << "mem_type=" << QString::fromStdString(std::string(mem_type));
//    qDebug() << "mem_manufacturer=" << QString::fromStdString(std::string(mem_manufacturer));
//    qDebug() << "mem_serial=" << QString::fromStdString(std::string(mem_serial));

    return value;
}
