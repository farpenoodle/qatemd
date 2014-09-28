#ifndef QATEMD_H
#define QATEMD_H

#include <QTcpServer>
#include <QTcpSocket>
#include "qatemhandler.h"

class qAtemD : public QTcpServer
{
    Q_OBJECT
public:
    explicit qAtemD(QObject *parent = 0);
    void startServer();
signals:

public slots:
    void readClient();
    void discardClient();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    qAtemHandler *atemHandler;
};


#endif // QATEMD_H
