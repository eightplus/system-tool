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

#include "filesystemworker.h"

#include <stddef.h>
#include <glibtop/mountlist.h>
#include <glibtop/fsusage.h>
/*For PRIu64*/
#include <inttypes.h>

#include <QString>
using std::string;

typedef struct _DISK_INFO
{
    char devname[256];
    char mountdir[256];
    char type[256];
    gint percentage;
    guint64 btotal;
    guint64 bfree;
    guint64 bavail;
    guint64 bused;
    gint valid;
} DISK_INFO;

std::string make_string(char *c_str)
{
    if (!c_str) {
        return string();
    }
    string s(c_str);
    g_free(c_str);
    return s;
}

static void fsusage_stats(const glibtop_fsusage *buf,
              guint64 *bused, guint64 *bfree, guint64 *bavail, guint64 *btotal,
              gint *percentage)
{
    guint64 total = buf->blocks * buf->block_size;

    if (!total) {
        /* not a real device */
        *btotal = *bfree = *bavail = *bused = 0ULL;
        *percentage = 0;
    } else {
        int percent;
        *btotal = total;
        *bfree = buf->bfree * buf->block_size;
        *bavail = buf->bavail * buf->block_size;
        *bused = *btotal - *bfree;
        /* percent = 100.0f * *bused / *btotal; */
        percent = 100 * *bused / (*bused + *bavail);
        *percentage = CLAMP(percent, 0, 100);
    }
}

DISK_INFO add_disk(const glibtop_mountentry *entry, gboolean show_all_fs)
{
    DISK_INFO disk;
    memset(&disk, 0, sizeof(disk));
    disk.valid = 0;
    glibtop_fsusage usage;
    guint64 bused, bfree, bavail, btotal;
    gint percentage;
    glibtop_get_fsusage(&usage, entry->mountdir);
    if (usage.blocks == 0) {
        return disk;
    }
    if(strcmp(entry->devname,"none")==0 || strcmp(entry->devname,"tmpfs")==0){
        return disk;
    }
    if(strstr(entry->type, "tmpfs")) {
        return disk;
    }
    fsusage_stats(&usage, &bused, &bfree, &bavail, &btotal, &percentage);
    memcpy(disk.devname, entry->devname, strlen(entry->devname));
    memcpy(disk.mountdir, entry->mountdir, strlen(entry->mountdir));
    memcpy(disk.type, entry->type, strlen(entry->type));
    disk.percentage = percentage;
    disk.btotal = btotal;
    disk.bfree = bfree;
    disk.bavail = bavail;
    disk.bused = bused;
    disk.valid = 1;

    return disk;
}


FileSystemWorker::FileSystemWorker(QObject *parent)
    : QObject(parent)
{

}

FileSystemWorker::~FileSystemWorker()
{
    m_diskInfoList.clear();
}

void FileSystemWorker::onFileSystemListChanged()
{
    QStringList newDiskList;
    glibtop_mountentry *entries;
    glibtop_mountlist mountlist;
    guint i;
    gboolean show_all_fs = TRUE;
    entries = glibtop_get_mountlist(&mountlist, show_all_fs);
    for (i = 0; i < mountlist.number; i++) {
        DISK_INFO disk = add_disk(&entries[i], show_all_fs);
        if (disk.valid == 1) {
            std::string formatted_dev = make_string(g_strdup(disk.devname));
            QString dev_name = QString::fromStdString(formatted_dev);
            newDiskList.append(dev_name);

            if (!this->isDeviceContains(dev_name)) {
                FileSystemData *info = new FileSystemData(this);
                info->setDevName(dev_name);

                std::string formatted_mountdir(make_string(g_strdup(disk.mountdir)));
                std::string formatted_type(make_string(g_strdup(disk.type)));
                std::string formatted_btotal(make_string(g_strdup(g_format_size_full(disk.btotal, G_FORMAT_SIZE_DEFAULT))));
                std::string formatted_bfree(make_string(g_strdup(g_format_size_full(disk.bfree, G_FORMAT_SIZE_DEFAULT))));
                std::string formatted_bavail(make_string(g_strdup(g_format_size_full(disk.bavail, G_FORMAT_SIZE_DEFAULT))));
                std::string formatted_bused(make_string(g_strdup(g_format_size_full(disk.bused, G_FORMAT_SIZE_DEFAULT))));
                info->updateDiskInfo(QString::fromStdString(formatted_mountdir), QString::fromStdString(formatted_type), QString::fromStdString(formatted_btotal), QString::fromStdString(formatted_bfree), QString::fromStdString(formatted_bavail), QString::fromStdString(formatted_bused), disk.percentage);
                this->addDiskInfo(dev_name, info);
            }
            else {//update info which had exists
                FileSystemData *info = this->getDiskInfo(dev_name);
                if (info) {
                    std::string formatted_mountdir(make_string(g_strdup(disk.mountdir)));
                    std::string formatted_type(make_string(g_strdup(disk.type)));
                    std::string formatted_btotal(make_string(g_strdup(g_format_size_full(disk.btotal, G_FORMAT_SIZE_DEFAULT))));
                    std::string formatted_bfree(make_string(g_strdup(g_format_size_full(disk.bfree, G_FORMAT_SIZE_DEFAULT))));
                    std::string formatted_bavail(make_string(g_strdup(g_format_size_full(disk.bavail, G_FORMAT_SIZE_DEFAULT))));
                    std::string formatted_bused(make_string(g_strdup(g_format_size_full(disk.bused, G_FORMAT_SIZE_DEFAULT))));
                    info->updateDiskInfo(QString::fromStdString(formatted_mountdir), QString::fromStdString(formatted_type), QString::fromStdString(formatted_btotal), QString::fromStdString(formatted_bfree), QString::fromStdString(formatted_bavail), QString::fromStdString(formatted_bused), disk.percentage);
                }
            }
        }
    }

    //remove the device whice not exists anymore
    for (auto device : m_diskInfoList.keys()) {
        bool foundDevice = false;
        for (auto devName : newDiskList) {
            if (devName == device) {
                foundDevice = true;
                break;
            }
        }

        if (!foundDevice) {
            m_diskInfoList.remove(device);//or erase???
        }
    }

    g_free(entries);
}

FileSystemData *FileSystemWorker::getDiskInfo(const QString &devname)
{
    return m_diskInfoList.value(devname, nullptr);
}

QList<FileSystemData *> FileSystemWorker::diskInfoList() const
{
    return m_diskInfoList.values();
}

void FileSystemWorker::addDiskInfo(const QString &devname, FileSystemData *info)
{
    if (!m_diskInfoList.contains(devname)) {
        m_diskInfoList[devname] = info;
    }
}

bool FileSystemWorker::isDeviceContains(const QString &devname)
{
    return m_diskInfoList.contains(devname);
}
