#include "thread.h"

#include <QIODevice>
#include <QTextStream>

Thread::Thread(qintptr socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor) {
}

void Thread::run()
{
    socket = new QTcpSocket();

    // Opens socket based on the socketDescriptor passed from the
    // main thread
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        emit error(socket->error());
        return;
    }

    cout << "Receiving client\n";

    cmdHandler << new SendFile(socket);

<<<<<<< HEAD
        cout << "Sending file\n";
        //sendFile("/home/joshua/Documents/examples/ROICDATA.raw");
        sendFile("/home/joshua/Documents/testbed-images/image1.envi");

    }
=======
    cout << "starting loop\n";
>>>>>>> e8ef4b3380ad7d72aa8d4bf8ba79285ba86026a3

    while (true) {
        if (socket->waitForReadyRead()) {
            onReadyRead();
        } else {
            break;
        }
    }
}

void Thread::onReadyRead() {

    /*
     * Note: there could be a memory leak here if malicious clients keep sending
     * messages that have no '\0' character to signal the end of a message.
     */

    // Acccept all incoming data into buffer
    inBuffer.append(socket->readAll());

    /*
     * If there is a terminating character, assume a full command was
     * sent and try to execute it */
    while (inBuffer.contains('\0')) {
        // This reads the QByteArray up to the first \0 character
        QString cmdName = QString(inBuffer);

        // This deletes all the characters that were just read
        inBuffer.remove(0, inBuffer.indexOf('\0')+1);

        // endl is necessary here
        cout << "Received command: " <<  cmdName.toStdString() << "\n";
        cmdHandler.tryRun(cmdName, QByteArray("/home/jmanders/Documents/LV-image-server/examples/ENVI_ROICWARM/ROICDATA.raw"));
    }

}

bool Thread::sendFile(QString path) {
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
        cout << 100.0 * float(amountRead) / file.size() << "\n";
        if (socket->state() == QAbstractSocket::ConnectedState) {
            qint64 written = socket->write(outBuffer);

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
