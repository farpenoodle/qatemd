#include <QDebug>
#include <QTimer>
#include "qatemcmd.h"
#include "libqatemcontrol/qatemconnection.h"

qAtemCmd::qAtemCmd(QUrlQuery quer,QAtemConnection *atemC,QObject *parent) :
    QObject(parent),atemConnection(atemC)
{
    query = quer;
    //qDebug() << query.toString();
    m_delay = 0;
    if (query.hasQueryItem("delay")) {
        m_delay = query.queryItemValue("delay").toInt();
    }
    m_action = "";
    if (query.hasQueryItem("action")) {
        m_action = query.queryItemValue("action");
    }
    m_input = 1;
    if (query.hasQueryItem("input")) {
        m_input = query.queryItemValue("input").toInt();
    }
    qDebug() << "Action: " << m_action << " | Input: " << m_input << " | Delay: " << m_delay;
}

void qAtemCmd::execute() {

    //atemConnection->changeProgramInput(4);
    QTimer::singleShot(m_delay, this, SLOT(fire()));

}

void qAtemCmd::fire() {
    doAction();
    this->deleteLater();
}

void qAtemCmd::doAction() {
    if (m_action == "switch") {
        doSwitch();
    } else if (m_action == "switchNoTrans") {
        doSwitchNoTrans();
    }
}

void qAtemCmd::doSwitch() {
    atemConnection->changePreviewInput(m_input);
    atemConnection->doAuto();
}

void qAtemCmd::doSwitchNoTrans() {
    atemConnection->changeProgramInput(m_input);
}
