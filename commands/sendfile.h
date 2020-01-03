#ifndef SENDFILE_H
#define SENDFILE_H

#include <QFile>
#include <QDataStream>
#include <iostream>

#include "command.h"

using namespace std;

class SendFile : public Command {
    Q_OBJECT

public:
    using Command::Command;

    QString getName() {
        return "send";
    }

    /*
     * Sends the requested file.
     * Argument corresponds to requested data file located in dataDir in
     * this case.
     */
    bool run(QString arg) {
        QFile file{dataDir.filePath(arg)};
        if (!file.open(QIODevice::ReadOnly))
                return false;

        cout << "Starting send\n";

        while (!file.atEnd()) {

            QByteArray outBuffer;
            QDataStream outStream(&outBuffer, QIODevice::WriteOnly);

            // Reads one full frame worth of pixels (480*640 is arbitrary)
            for (int i=0; i < 480*640; i++) {
                uint16_t pix;
                // Reads two bytes from the file as a 16 bit integer
                file.read(reinterpret_cast<char *>(&pix), sizeof(pix));
                outStream << pix;
            }

            if (outSocket->state() == QAbstractSocket::ConnectedState) {
                outSocket->write(outBuffer);
                outSocket->waitForBytesWritten();
            } else {
                // Exits if the socket is disconnected
                cout << "Client disconnected unexpectedly\n";
                break;
            }
        }
        file.close();

        cout << "Send complete\n";

        return true;
    }
};

#endif // SENDFILE_H
