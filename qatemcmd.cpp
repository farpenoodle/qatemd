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
