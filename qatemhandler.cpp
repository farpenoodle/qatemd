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
