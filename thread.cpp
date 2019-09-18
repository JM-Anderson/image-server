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

    for (int i=0; i < 1; i++) {
        if (!(socket->state() == QAbstractSocket::ConnectedState))
            break;

        cout << "Sending file\n";
        sendFile("/home/joshua/Documents/examples/ROICDATA.raw");
    }

    socket->disconnectFromHost();
    socket->waitForDisconnected();
    cout << "\n\n";
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
