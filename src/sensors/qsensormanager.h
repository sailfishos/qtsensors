/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSENSORMANAGER_H
#define QSENSORMANAGER_H

#include "qsensor.h"

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

class QSensorBackend;
class QSensorBackendFactory;
class QSensorPluginInterface;

class Q_SENSORS_EXPORT QSensorManager
{
public:
    // Register a backend (call this from a plugin)
    static void registerBackend(const QByteArray &type, const QByteArray &identifier, QSensorBackendFactory *factory);
    static void unregisterBackend(const QByteArray &type, const QByteArray &identifier);

    static bool isBackendRegistered(const QByteArray &type, const QByteArray &identifier);

    // Create a backend (uses the type and identifier set in the sensor)
    static QSensorBackend *createBackend(QSensor *sensor);

    static void setDefaultBackend(const QByteArray &type, const QByteArray &identifier);
};

class Q_SENSORS_EXPORT QSensorBackendFactory
{
public:
    virtual QSensorBackend *createBackend(QSensor *sensor) = 0;
protected:
    ~QSensorBackendFactory() {}
};

QT_END_NAMESPACE
QT_END_HEADER

#endif

