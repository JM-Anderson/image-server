#include <QCoreApplication>
#include <QtCore>
#include <QtNetwork>

#include <iostream>

#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    static const quint16 DEFAULT_PORT = 1234;
    static const QString DEFAULT_PORT_STR = "1234";
    static const QString DEFAULT_DATA_DIR = "/home/joshua/Documents/testbed-images";

    QCoreApplication app(argc, argv);

    // Sets up command line options and parses them
    QCommandLineParser cmdParser;
    cmdParser.addOptions({
        {{"p", "port"},
            "Sets the port number",
            "port",
            DEFAULT_PORT_STR},
        {{"d", "data-directory"},
            "Sets the directory clients can request data from",
            "directory",
            DEFAULT_DATA_DIR}
    });
    cmdParser.process(app);

    // Get IP adress
    const QHostAddress ipAddress = QHostAddress::AnyIPv4;

    // Get port number
    bool valid;
    quint16 port = cmdParser.value("port").toUInt(&valid);
    if (!valid) {
        cout << "Invalid port: " << cmdParser.value("port").toStdString()
             << "\nResorting to default port: " << DEFAULT_PORT_STR.toStdString()
             << "\n";
        port = DEFAULT_PORT;
    }

    // Get data directory
    QDir dataDir{cmdParser.value("data-directory")};
    if (!dataDir.exists()) {
        throw invalid_argument(
                    QString("Invalid directory: %1").arg(cmdParser.value("data-directory"))
                    .toStdString());
    }

    // Start server
    Server server{dataDir};
    if (!server.listen(ipAddress, port)) {
        qDebug() << "Unable to start the server: " << server.errorString();
        exit(1);
    }

    cout << "The server is running on\n"
            "IP: " << ipAddress.toString().toStdString() << "\n"
            "port: " << server.serverPort() << "\n\n";

    return app.exec();
}
