/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "diskutil.h"

#include <QThread>

#include <XSys>

namespace DiskUtil
{
XSys::Result MountPartition(const QString &partition)
{
    // mount

    QString mountPoint = XSys::DiskUtil::MountPoint(partition);
    if (mountPoint.isEmpty()) {
        mountPoint = QString("%1").arg(XSys::FS::TmpFilePath("deepin-boot-maker"));
    }

    XSys::Result ret = XSys::SynExec("mkdir", QString(" -p %1").arg(mountPoint));
    if (!ret.isSuccess()) { return ret; }

    ret = XSys::SynExec("chmod",  " a+wrx " + mountPoint);
    if (!ret.isSuccess()) { return ret; }

    QString mountCmd = "%1 %2";
    QString remountCmd = "-o flush,rw,utf8=1,sync,nodev,nosuid, %1 %2";
    XSys::DiskUtil::UmountDisk(partition);
    QThread::sleep(3);
    XSys::SynExec("mount",  mountCmd.arg(partition).arg(mountPoint));
    QThread::sleep(3);
    XSys::SynExec("mount",  remountCmd.arg(partition).arg(mountPoint));
    return XSys::Result(XSys::Result::Success, "", mountPoint);
}
}
//#include <libudev.h>
//#include <stdio.h>

//static struct udev_device *
//get_child(
//    struct udev *udev, struct udev_device *parent, const char *subsystem)
//{
//    struct udev_device *child = NULL;
//    struct udev_enumerate *enumerate = udev_enumerate_new(udev);

//    udev_enumerate_add_match_parent(enumerate, parent);
//    udev_enumerate_add_match_subsystem(enumerate, subsystem);
//    udev_enumerate_scan_devices(enumerate);

//    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
//    struct udev_list_entry *entry;

//    udev_list_entry_foreach(entry, devices) {
//        const char *path = udev_list_entry_get_name(entry);
//        child = udev_device_new_from_syspath(udev, path);
//        break;
//    }

//    udev_enumerate_unref(enumerate);
//    return child;
//}

//static void enumerate_usb_mass_storage(struct udev *udev)
//{
//    struct udev_enumerate *enumerate = udev_enumerate_new(udev);

//    udev_enumerate_add_match_subsystem(enumerate, "scsi");
//    udev_enumerate_add_match_property(enumerate, "DEVTYPE", "scsi_device");
//    udev_enumerate_scan_devices(enumerate);

//    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
//    struct udev_list_entry *entry;

//    udev_list_entry_foreach(entry, devices) {
//        const char *path = udev_list_entry_get_name(entry);
//        struct udev_device *scsi = udev_device_new_from_syspath(udev, path);

//        struct udev_device *block = get_child(udev, scsi, "block");
//        struct udev_device *scsi_disk = get_child(udev, scsi, "scsi_disk");

//        struct udev_device *usb
//            = udev_device_get_parent_with_subsystem_devtype(
//                  scsi, "usb", "usb_device");

//        if (block && scsi_disk && usb) {
//            printf("block = %s, usb = %s:%s, scsi = %s\n",
//                   udev_device_get_devnode(block),
//                   udev_device_get_sysattr_value(usb, "idVendor"),
//                   udev_device_get_sysattr_value(usb, "idProduct"),
//                   udev_device_get_sysattr_value(scsi, "vendor"));
//        }

//        if (block) {
//            udev_device_unref(block);
//        }

//        if (scsi_disk) {
//            udev_device_unref(scsi_disk);
//        }

//        udev_device_unref(scsi);
//    }

//    udev_enumerate_unref(enumerate);
//}

