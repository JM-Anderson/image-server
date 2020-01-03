#ifndef RELAY_H
#define RELAY_H

#include <QHostAddress>
#include <iostream>

#include "command.h"

using namespace std;

class Relay : public Command {
    Q_OBJECT

public:
    using Command::Command;

    QString getName() {
        return "relay";
    }

    /*
     * Relays any incoming data from inSocket to the client, outSocket
     */
    bool run(QString arg) {
        QStringList args = arg.split(" ");

        if (args.length() != 2) {
            cout << "Format: relay <address> <port>\n";
            return false;
        }

        QHostAddress address = QHostAddress(args[0]);
        quint16 port = args[1].toUShort();

        inSocket->connectToHost(address, port);

        if (!inSocket->waitForConnected()) {
            cout << "Could not connect.\n";
            return false;
        }

        // Loops until either party is disconnected
        while (true) {
            if (inSocket->state() == QAbstractSocket::SocketState::UnconnectedState) {
                cout << "Ended relay\n";
                return true;
            }

            if (outSocket->state() == QAbstractSocket::SocketState::UnconnectedState) {
                cout << "Receiving client disconnected unexpectedly\n";
                return false;
            }

            if (inSocket->waitForReadyRead()) {
                QByteArray data = inSocket->readAll();
                outSocket->write(data);
                outSocket->waitForBytesWritten();
            }
        }
    }
};

#endif // RELAY_H
