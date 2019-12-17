#ifndef COMMAND_H
#define COMMAND_H

#include <QTcpSocket>

class Command : public QObject
{
    Q_OBJECT

public:
    Command(QTcpSocket* socket, QObject* parent = nullptr) : QObject(parent), socket(socket) {}
    virtual ~Command() {}

    /*
     * The method that runs when the command is called
     */
    virtual bool run(QByteArray args) = 0;

    /*
     * Returns the name of the command as a char array
     */
    virtual QString getName() = 0;

protected:
    QTcpSocket* socket;

};

#endif // COMMAND_H
