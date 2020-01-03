#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <iostream>

#include "command.h"
#include "commandhandler.h"

#include "commands/sendfile.h"
#include "commands/relay.h"
#include "commands/threadinfo.h"
#include "commands/changethread.h"

using namespace std;

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(qintptr socketDescriptor, QDir dataDir, uint threadID, QObject *parent);

    void run() override;

    bool disabled = true;

    bool isBlocking() {
        return blocking;
    }

    // The number assigned to this thread by the server
    const uint threadID;

signals:
    void error(QTcpSocket::SocketError socketError);
    void requestChangeThread(uint ThreadID);
    void clientDisconnected(uint ThreadID);
    void startedBlocking(uint ThreadID);
    void finishedBlocking(uint ThreadID);

private:
    // Data Location
    QDir dataDir;

    // TCP Socket information - for sending data
    qintptr socketDescriptor;
    QTcpSocket* outSocket;

    // TCP client for receiving data
    QTcpSocket* inSocket;

    // Handles user commands
    void getCommand();
    CommandHandler cmdHandler;
    ChangeThread* changeThreadCmd;

    // Is true if an operation is currently underway
    bool blocking = false;
};

#endif
