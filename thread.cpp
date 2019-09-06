#include "thread.h"

Thread::Thread(qintptr socketDescriptor, const QString &fortune, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), text(fortune) {
}

void Thread::run()
{
    QTcpSocket socket;

    // Opens socket based on the socketDescriptor passed from the
    // main thread
    if (!socket.setSocketDescriptor(socketDescriptor)) {
        emit error(socket.error());
        return;
    }

    cout << "Receiving client\n";

    QFile file("in.txt");
    if (!file.open(QIODevice::ReadOnly))
            return;

    qint64 fileSize = file.size();
    qint64 amountRead = 0;
    qint64 chunkSize = 8;

    while (amountRead < fileSize){
        QByteArray data;
        data = file.read(chunkSize);
        amountRead += data.size();
        socket.write(data);
        socket.waitForBytesWritten();
    }
    file.close();
    //socket.flush();

    // Disconnects socket
    //socket.disconnectFromHost();
    //socket.waitForDisconnected();
}
