/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "vboxaccelerometer.h"
#include <QDebug>
#include <QtGlobal>
#include <QScreen>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <QFile>
#include <unistd.h>
#include <QtCore/qsocketnotifier.h>


#define FB_PATH "/sys/class/graphics/fb0/virtual_size"
// #define FB_PATH "/sys/devices/virtual/graphics/fbcon/rotate"
/*
 *
    0 - normal orientation (0 degree)
    1 - clockwise orientation (90 degrees)
    2 - upside down orientation (180 degrees)
    3 - counterclockwise orientation (270 degrees)
*/
char const * const VBoxAccelerometer::id("vbox.accelerometer");


VBoxAccelerometer::VBoxAccelerometer(QSensor *sensor)
    : VBoxCommon(sensor),
      currentOrientation(Qt::PrimaryOrientation),
      inotifyWatcher(-1),
      inotifyFileDescriptor(-1),
      notifier(0)
{
    qDebug();
    setReading<QAccelerometerReading>(&m_reading);
    addDataRate(1, 100); // 100Hz
    setupWatcher();
    readFb();
}

VBoxAccelerometer::~VBoxAccelerometer()
{
    cleanupWatcher();
}

void VBoxAccelerometer::poll()
{
    m_reading.setTimestamp(getTimestamp());

    qreal x = 0;
    qreal y = 9.8;
    //TODO determine if tbj or sbj

    switch(currentOrientation) {
    case Qt::LandscapeOrientation:// tbj
        y = 9.8;
        break;
    case Qt::PortraitOrientation:// sbj
        y = 9.8;
        break;
    default:
        break;
    };
    m_reading.setX(x);
    m_reading.setY(y); // facing the user, gravity goes here
    m_reading.setZ(0);
    newReadingAvailable();
}

void VBoxAccelerometer::cleanupWatcher()
{
    if (notifier) {
        delete notifier;
        notifier = 0;
    }

    if (inotifyWatcher != -1) {
        inotify_rm_watch(inotifyFileDescriptor, inotifyWatcher);
        inotifyWatcher = -1;
    }

    if (inotifyFileDescriptor != -1) {
        close(inotifyFileDescriptor);
        inotifyFileDescriptor = -1;
    }
}

void VBoxAccelerometer::setupWatcher()
{
    qDebug();
    if (inotifyFileDescriptor == -1
            && (inotifyFileDescriptor = inotify_init()) == -1) {
        qWarning() << "inotify_init failed";
        return;
    }

    if (inotifyWatcher == -1
            && (inotifyWatcher = inotify_add_watch(inotifyFileDescriptor, FB_PATH, IN_MODIFY)) == -1) {
        close(inotifyFileDescriptor);
        qWarning() << "inotify_add_watch failed";
        return;
    }

    if (notifier == 0) {
        notifier = new QSocketNotifier(inotifyFileDescriptor, QSocketNotifier::Read);
        connect(notifier, SIGNAL(activated(int)), this, SLOT(onInotifyActivated()));
    }
}

void VBoxAccelerometer::onInotifyActivated()
{
    inotify_event event;
    if (read(inotifyFileDescriptor, (void *)&event, sizeof(event)) > 0
            && event.wd == inotifyWatcher) {
        // Have to do this, otherwise I can't get further notification
        inotify_rm_watch(inotifyFileDescriptor, inotifyWatcher);
        inotifyWatcher = inotify_add_watch(inotifyFileDescriptor, FB_PATH, IN_MODIFY);
        qDebug();
        readFb();
    }
}

void VBoxAccelerometer::readFb()
{
    QFile fbFile(FB_PATH);
    if (fbFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&fbFile);
        QString line = in.readLine();
        int width = line.section(",",0,0,QString::SectionSkipEmpty).toInt();
        int height = line.section(",",1,1,QString::SectionSkipEmpty).toInt();
        qDebug() << "width" << width << "height" << height;
        if (width > height) {
            currentOrientation = Qt::LandscapeOrientation;// tbj
            qDebug() << "landscape/tbj";
        } else {
            currentOrientation = Qt::PortraitOrientation;// sbj
            qDebug() << "portrait/sbj";
        }
        fbFile.close();
    }
}

