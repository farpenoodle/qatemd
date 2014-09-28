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
