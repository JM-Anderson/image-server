#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <iostream>

#include "command.h"
#include "client.h"
#include "commandhandler.h"
#include "commands/sendfile.h"

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
    QTcpSocket* socket;

    // TCP client for receiving data
    Client* client;
    bool relayMode = false;

    // Buffer for holding incoming data
    QByteArray inBuffer;

    // Handles incoming commands
    CommandHandler cmdHandler;

private slots:
    void onReadyRead();
};

#endif
