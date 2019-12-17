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

    bool run(QByteArray args) {
        QString path(args);

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
                return false;

        // Number of bytes read so far
        qint64 amountRead = 0;

        while (!file.atEnd()){
            QByteArray outBuffer;
            QDataStream outStream(&outBuffer, QIODevice::WriteOnly);

            for (int i=0; i < 480*640; i++) {
                uint16_t pix;
                file.read(reinterpret_cast<char *>(&pix), sizeof(pix));
                outStream << pix;
            }
            amountRead += outBuffer.size();
            cout << "Amount read: " << amountRead << "\n";

            if (socket->state() == QAbstractSocket::ConnectedState) {
                qint64 written = socket->write(outBuffer);

                cout << "Bytes written: " << written << "\n";

                // Forces socket to write data immediately rather than
                // add to buffer
                socket->waitForBytesWritten();
            } else {
                break;
            }
        }
        file.close();

        return true;
    }
};

#endif // SENDFILE_H
