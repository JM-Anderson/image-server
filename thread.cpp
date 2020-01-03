#include "thread.h"

Thread::Thread(qintptr socketDescriptor, QDir dataDir, QObject *parent)
    : QThread(parent), dataDir(dataDir), socketDescriptor(socketDescriptor) {
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

    // Setting up command handler
    cmdHandler << new SendFile(outSocket, inSocket, dataDir);
    cmdHandler << new Relay(outSocket, inSocket, dataDir);

    // Main loop
    while (true) {
        getCommand();
    }
}

// Takes console input then runs the corresponding command
void Thread::getCommand() {
    QTextStream s(stdin);
    QString input = s.readLine();

    if (input.trimmed() == "") {
        return;
    }

    // Takes the first word as the command name and reformats other words
    // to a standard form
    QStringList splitArgs = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QString cmdName = splitArgs[0];
    splitArgs.removeAt(0);
    QString args = splitArgs.join(" ");

    if (cmdHandler.tryRun(cmdName, args)) {
        cout << cmdName.toStdString() << " succeeded\n";
    }
}
