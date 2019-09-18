#include <QCoreApplication>
#include <QtCore>
#include <QtNetwork>

#include <iostream>

#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QHostAddress ipAddress = QHostAddress::AnyIPv4;
    quint16 port = 1234;

    QStringList argList = QCoreApplication::arguments();
    if (argList.size() > 1) {
        QString portStr =  argList.at(1);

        bool flag;
        uint num = portStr.toUInt(&flag);
        if(flag){
            port = num;
        }
    }

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
