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

#include "qatemd.h"
#include "qatemhandler.h"

qAtemD::qAtemD(QObject *parent) :
    QTcpServer(parent)
{
    atemHandler = new qAtemHandler(this);
}

void qAtemD::startServer()
{
    int port = 895;

    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening on port " << port << "...";
    }
}

void qAtemD::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    //qDebug() << socketDescriptor << " Connecting...";

    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socketDescriptor);

}

void qAtemD::readClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << "HTTP/1.0 200 Ok\r\n"
                "Content-Type: text/html; charset=\"utf-8\"\r\n"
                "\r\n"
                "<h1>Nothing to see here</h1>\n";
            socket->close();


            //qDebug() << socketDescriptor <<  "Wrote to client";

            QStringList tokens2 = tokens[1].split("/");
            QStringList tokens3 = tokens[1].split("?");

            if (tokens2[1] == "atem" && tokens3.length() > 1) {
               atemHandler->handleRequest(tokens3[1]);
            }

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
                //qDebug() << "Connection closed";
            }
        } else {
            socket->close();
            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
                //qDebug() << "Connection closed";
            }
        }
    }
}

void qAtemD::discardClient()
     {
         QTcpSocket* socket = (QTcpSocket*)sender();
         socket->deleteLater();

         //qDebug() << "Connection closed";
     }
