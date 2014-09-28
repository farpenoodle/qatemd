#ifndef QATEMCMD_H
#define QATEMCMD_H

#include <QObject>
#include "libqatemcontrol/qatemconnection.h"
#include <QUrlQuery>

class qAtemCmd : public QObject
{
    Q_OBJECT
public:
    explicit qAtemCmd(QUrlQuery quer,QAtemConnection *atemC = 0, QObject *parent = 0);


    void execute();
    void doAction();

    void doSwitch();
    void doSwitchNoTrans();

signals:

public slots:
    void fire();

private:
    QString m_action;
    quint8 m_input;
    int m_delay;
    QUrlQuery query;
    QAtemConnection *atemConnection;

};

#endif // QATEMCMD_H
