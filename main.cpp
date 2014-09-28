#include <QCoreApplication>
#include <QCommandLineParser>
#include "qatemd.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    qAtemD server;

    server.startServer();

    return a.exec();
}
