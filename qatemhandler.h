#ifndef QATEMHANDLER_H
#define QATEMHANDLER_H

#include <QObject>
#include <QMap>
#include "libqatemcontrol/qatemconnection.h"
#include "qatemcmd.h"

class qAtemHandler : public QObject
{
    Q_OBJECT
public:
    explicit qAtemHandler(QObject *parent = 0);
    void handleRequest(QString);
    void test();
    void addToQueue(QString,QUrlQuery);
    void executeQueue(QString);

signals:

public slots:

    void handleConnect();
    void handleError(QString);

private:
    QMap<QString, QAtemConnection*> atemConnections;
    QMap<QString, bool> atemConnected;
    QMap<QString, QList<qAtemCmd*> > atemQueue;

};

#endif // QATEMHANDLER_H
