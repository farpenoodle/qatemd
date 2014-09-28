/**********************************************************************************

Copyright (c) 2012, Tan Yu Sheng
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************************/

#include "qatemhandler.h"
#include "qatemcmd.h"
#include <QDebug>
#include <QUrlQuery>
#include <QHostAddress>
#include <QMap>
#include <QList>
#include <QListIterator>

qAtemHandler::qAtemHandler(QObject *parent) :
    QObject(parent)
{
}

void qAtemHandler::handleRequest(QString request) {
    QUrlQuery query(request);
    if (!query.hasQueryItem("atem")) {
         qDebug() << "No ATEM specified";
         return;
    }
    QHostAddress atemHost = QHostAddress(query.queryItemValue("atem"));
    QString atemName = atemHost.toString();

    if(atemHost.isNull())
    {
        qDebug() << "Invalid switcher address";
        return;
    }

    if (!atemConnections.contains(atemName)){
        atemConnections[atemName] = new QAtemConnection();
        atemConnected[atemName] = false;
        connect(atemConnections[atemName], SIGNAL(socketError(QString)),
                this, SLOT(handleError(QString)));
        connect(atemConnections[atemName], SIGNAL(connected()),
                this, SLOT(handleConnect()));

    }
    if (!atemConnected[atemName]) {
        qDebug() << "Connecting to ATEM (" << atemName << ")";
        atemConnections[atemName]->connectToSwitcher(atemHost);
    }

    addToQueue(atemName,query);

}

void qAtemHandler::handleConnect() {
    QString atemName = ((QAtemConnection*)sender())->getHostAddress().toString();
    atemConnected[atemName] = true;
    qDebug() << "Connected to ATEM (" << atemName << ")";
    executeQueue(atemName);
}

void qAtemHandler::handleError(QString error) {
    qDebug() << error;
}

void qAtemHandler::addToQueue(QString atemName, QUrlQuery query) {
    qAtemCmd *newCmd = new qAtemCmd(query,atemConnections[atemName],this);
    atemQueue[atemName].append(newCmd);
    if (atemConnected[atemName]) {
        executeQueue(atemName);
    }
}

void qAtemHandler::executeQueue(QString atemName) {
    QListIterator<qAtemCmd* > i(atemQueue[atemName]);
    while (i.hasNext()) {
        qAtemCmd *cmd = i.next();
        cmd->execute();
    }
    atemQueue[atemName].clear();
}
