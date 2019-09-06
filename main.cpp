#include <QCoreApplication>
#include <QtCore>
#include <QtNetwork>

#include <iostream>

#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    const QHostAddress ipAddress = QHostAddress(QHostAddress::LocalHost);
    const quint16 port = 1234;

    QCoreApplication app(argc, argv);

    Server server;

    if (!server.listen(ipAddress, port)) {
        qDebug() << "Unable to start the server: " << server.errorString();
        exit(1);
    }

    cout << "The server is running on\n"
            "IP: " << ipAddress.toString().toStdString() << "\n"
            "port: " << server.serverPort() << "\n\n";

    return app.exec();
}
