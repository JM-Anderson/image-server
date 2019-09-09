#ifndef COMMAND_H
#define COMMAND_H

#include <QTcpSocket>

class Command : public QObject
{
    Q_OBJECT

public:
    Command(QObject* parent, QTcpSocket* socket) : QObject(parent), socket(socket) {}
    virtual ~Command() {}

    virtual void init() {}
    virtual void run(QByteArray args) = 0;

    char getCmdByte() { return cmdByte; }

protected:
    char cmdByte;
    QTcpSocket* socket;

};

#endif // COMMAND_H
