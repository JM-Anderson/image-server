#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <iostream>

#include "command.h"

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
    qintptr socketDescriptor;
    QTcpSocket* socket;

private slots:
    void onReadyRead();
    bool sendFile(QString path);
};

#endif
