#ifndef SERVER_H
#define SERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QDir>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QDir dataDir, QObject *parent = nullptr);

private:
    QDir dataDir;

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif
