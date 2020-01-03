#include "thread.h"

Thread::Thread(qintptr socketDescriptor, QDir dataDir, uint threadID, QObject *parent)
    : QThread(parent), threadID(threadID), dataDir(dataDir), socketDescriptor(socketDescriptor) {
}

/*
 * Runs on thread creation (when a new client is received)
 */
void Thread::run()
{
    outSocket = new QTcpSocket();
    inSocket = new QTcpSocket();

    // Opens socket based on the socketDescriptor passed from the
    // main thread
    if (!outSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(outSocket->error());
        return;
    }
    cout << "Receiving client\n";

    // Add commands to command handler
    // These parameters (outSocket, inSocket, dataDir, threadID) should probably be
    // consolidated into a single "Context" struct
    cmdHandler << new SendFile(outSocket, inSocket, dataDir, threadID);
    cmdHandler << new Relay(outSocket, inSocket, dataDir, threadID);
    cmdHandler << new ThreadInfo(outSocket, inSocket, dataDir, threadID);
    cmdHandler << new ChangeThread(outSocket, inSocket, dataDir, threadID);


    // Main loop
    while (true) {
        if (outSocket->state() == QAbstractSocket::SocketState::UnconnectedState) {
            cout << "Client disconnected\n";
            break;
        }

        getCommand();
    }

    emit clientDisconnected(threadID);
}

// Takes console input then runs the corresponding command
void Thread::getCommand() {
    if (disabled)
        return;

    cout << "\nSelected thread:\n";
    cmdHandler.getCmd("current")->run("");

    QTextStream s(stdin);
    cout << ">> ";
    QString input = s.readLine();

    if (input.trimmed() == "") {
        return;
    }

    // Takes the first word as the command name and reformats other words
    // to standard form
    QStringList splitArgs = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QString cmdName = splitArgs[0];
    splitArgs.removeAt(0);
    QString args = splitArgs.join(" ");

    Command* cmd = cmdHandler.getCmd(cmdName);
    if (cmd != nullptr) {
        if (cmd->isBlocking()) {
            blocking = true;
            emit startedBlocking(threadID);
        }

        cmd->run(args);
        blocking = false;
        emit finishedBlocking(threadID);

        if (cmd->getRequestedThread() >= 0) {
            disabled = true;
            emit requestChangeThread(uint(cmd->getRequestedThread()));
        }
        cmd->reset();
    }
}
