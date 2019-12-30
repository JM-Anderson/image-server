#include "thread.h"

#include <QIODevice>
#include <QTextStream>

Thread::Thread(qintptr socketDescriptor, QDir dataDir, QObject *parent)
    : QThread(parent), dataDir(dataDir), socketDescriptor(socketDescriptor) {
}

/*
 * Runs on thread creation (when a new client is received)
 */
void Thread::run()
{
    socket = new QTcpSocket();
    client = new Client(this);

    // Opens socket based on the socketDescriptor passed from the
    // main thread
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        emit error(socket->error());
        return;
    }
    cout << "Receiving client\n";

    // Setting up command handler
    cmdHandler << new SendFile(socket, dataDir);

    cmdHandler.tryRun("sendfile", "image1.envi");

    // Main loop - waits for then executes commands
    while (true) {
        QTextStream s(stdin);
        QString input = s.readLine();
        if (input == "whatever") {
            client->tcpConnect(QHostAddress::LocalHost, 1235);
            while (true) {
                if (client->waitForReadyRead()) {
                    client->readTcpData();

                }

            }
        }
        //if (socket->waitForReadyRead()) {
        //    onReadyRead();
        //}
    }
}

void Thread::onReadyRead() {
    /*
     * Note: there could be a memory leak here if malicious clients keep sending
     * messages that have no '\0' character to signal the end of a message.
     */

    // Acccept all incoming data into buffer
    inBuffer.append(socket->readAll());

    cout << "Received message";

    /*
     * If there is a terminating character, assume a full command was
     * sent and try to execute it */
    while (inBuffer.contains('\0')) {
        QStringList cmd = QString(inBuffer).split(':');

        QString cmdName = cmd[0];
        QString cmdArg;
        if (cmd.length() > 1)
            cmdArg = cmd[1];

        // This deletes all the characters that were just read
        inBuffer.remove(0, inBuffer.indexOf('\0')+1);

        // endl is necessary here
        cout << "Received command: " <<  cmdName.toStdString() << "\n";

        cmdHandler.tryRun(cmdName, cmdArg);
    }

}
