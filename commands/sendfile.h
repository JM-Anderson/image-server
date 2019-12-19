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
        return "sendfile";
    }

    /*
     * Sends the requested file.
     * Argument corresponds to requested data file located in dataDir in
     * this case.
     */
    bool run(QString arg) {
        // Checks to make sure file path is valid
        QFile file{dataDir.filePath(arg)};
        if (!file.open(QIODevice::ReadOnly))
                return false;

        // Loops until finished reading file
        while (!file.atEnd()){
            // Buffer that will contain all pixels
            QByteArray outBuffer;

            // Stream to easily write to buffer
            QDataStream outStream(&outBuffer, QIODevice::WriteOnly);

            // Reads one full frame worth of pixels
            // The amount read/sent at one time is arbitrary and 480*640
            // is only chosen because this is the framesize during testing.
            // Changing this value should not affect functionality.
            for (int i=0; i < 480*640; i++) {
                uint16_t pix;
                file.read(reinterpret_cast<char *>(&pix), sizeof(pix));
                outStream << pix;
            }

            if (socket->state() == QAbstractSocket::ConnectedState) {
                // Sends one frame of pixels through TCPSocket
                socket->write(outBuffer);

                // Forces socket to write data immediately rather than
                // adding to internal buffer
                socket->waitForBytesWritten();
            } else {
                // Exits if the socket is disconnected
                break;
            }
        }
        file.close();

        return true;
    }
};

#endif // SENDFILE_H
