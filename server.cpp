#include "server.h"
#include "thread.h"

Server::Server(QDir dataDir, QObject *parent) : QTcpServer(parent), dataDir(dataDir)
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Thread *thread = new Thread(socketDescriptor, dataDir, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
