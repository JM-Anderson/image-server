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

using namespace std;

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(qintptr socketDescriptor, QDir dataDir, QObject *parent);

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

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
};

#endif
