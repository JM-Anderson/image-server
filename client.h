#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QtNetwork>

using namespace std;

class Client : public QObject
{
    Q_OBJECT

public:
    Client(QObject *parent = nullptr);

    // Handles TCP connection
    void tcpConnect(const QHostAddress ipAddress, const quint16 port);
    char* readTcpData();
    bool waitForReadyRead();

private:
    QTcpSocket* socket = nullptr;
};

#endif // CLIENT_H
