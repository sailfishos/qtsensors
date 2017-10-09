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

#include "sensorfwpressuresensor.h"

char const * const SensorfwPressureSensor::id("sensorfw.pressuresensor");

SensorfwPressureSensor::SensorfwPressureSensor(QSensor *sensor)
    : SensorfwSensorBase(sensor),
      m_initDone(false)
{
    init();
    setDescription(QLatin1String("ambient pressure in pascals"));
    setReading<QPressureReading>(&m_reading);
    sensor->setDataRate(50);//set a default rate
}

void SensorfwPressureSensor::slotDataAvailable(const Unsigned& data)
{
    m_reading.setPressure(data.x());
    m_reading.setTimestamp(data.UnsignedData().timestamp_);
    newReadingAvailable();
}

bool SensorfwPressureSensor::doConnect()
{
    Q_ASSERT(m_sensorInterface);
    return QObject::connect(m_sensorInterface, SIGNAL(pressureChanged(Unsigned)),
                            this, SLOT(slotDataAvailable(Unsigned)));
}

QString SensorfwPressureSensor::sensorName() const
{
    return "pressuresensor";
}

void SensorfwPressureSensor::init()
{
    m_initDone = false;
    initSensor<PressureSensorChannelInterface>(m_initDone);
}

void SensorfwPressureSensor::start()
{
    if (reinitIsNeeded)
        init();
    SensorfwSensorBase::start();
}
