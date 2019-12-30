#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
}

void Client::tcpConnect(const QHostAddress ipAddress, const quint16 port) {
    socket->abort();
    inBuffer.clear();
    socket->connectToHost(ipAddress, port);
}

/*
 * If reimplementing this method, note that TCP data arrives in BigEndian
 * and hardware integers are most likely stored with LittleEndian. QDataStream
 * automatically reads in BigEndian and converts to the correct endianness.
*/
void Client::readTcpData()
{
    inBuffer.append(socket->readAll());
}

bool Client::waitForReadyRead()
{
    if (socket != nullptr) {
        return socket->waitForReadyRead();
    } else {
        return false;
    }

}