#include "thread.h"

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

    while (true) {
        socket->waitForReadyRead();
        onReadyRead();
    }
}

void Thread::onReadyRead() {
    QByteArray command = socket->readAll();

    /*
    if (command[0] == 'f') {
        sendFile("in.txt");
    } else if (command[0] == 'q') {
        // Disconnects socket
        socket->disconnectFromHost();

        // This will cause an error message: `QAbstractSocket::waitForDisconnected() is not allowed in UnconnectedState`
        // Simply ignore the error. It is a Qt bug.
        socket->waitForDisconnected();
    }
    */
}

bool Thread::sendFile(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
            return false;

    // Number of bytes read so far
    qint64 amountRead = 0;
    // Number of bytes to read at a time
    qint64 chunkSize = 8;

    while (amountRead < file.size()){
        QByteArray data;
        data = file.read(chunkSize);
        amountRead += data.size();
        socket->write(data);

        // Forces socket to write data immediately rather than
        // add to buffer
        socket->waitForBytesWritten();
    }
    file.close();

    return true;
}
