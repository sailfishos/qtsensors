/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtSensors 5.0
import Grue 1.0

Rectangle {
    width: 320
    height: 480
    color: "black"

    GrueSensor {
        id: sensor
        active: true
        onReadingChanged: {
            var percent = reading.chanceOfBeingEaten;
            var thetext = "";
            var theopacity = 0;
            if (percent === 0) {
                thetext = "It is light. You are safe from Grues.";
            }
            else if (percent === 100) {
                thetext = "You have been eaten by a Grue!";
                sensor.active = false;
                theopacity = 1;
            }
            else if (percent > 0) {
                thetext = "It is dark. You are likely to be eaten by a Grue. "
                        + "Your chance of being eaten by a Grue: "+percent+" percent.";
                theopacity = 0.05 + (percent * 0.001);
            }
            text.font.pixelSize = 30;
            text.text = "<p>" + thetext + "</p>";
            grueimg.opacity = theopacity;
        }
    }

    Text {
        id: text
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.right: parent.right
        text: "I can't tell if you're going to be eaten by a Grue or not. You're on your own!"
        wrapMode: Text.WordWrap
        font.pixelSize: 50
        color: "white"
    }

    Image {
        id: grueimg
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        source: "grue.png"
        opacity: 0
    }
}