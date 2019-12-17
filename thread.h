#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <iostream>

#include "command.h"
#include "commandhandler.h"
#include "commands/sendfile.h"

using namespace std;

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(qintptr socketDescriptor, QObject *parent);

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    // TCP Socket information
    qintptr socketDescriptor;
    QTcpSocket* socket;

    // Buffer for holding incoming data
    QByteArray inBuffer;

    // Handles incoming commands
    CommandHandler cmdHandler;

private slots:
    void onReadyRead();
    bool sendFile(QString path);
};

#endif
