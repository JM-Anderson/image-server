#ifndef COMMAND_H
#define COMMAND_H

#include <QTcpSocket>
#include <QDir>

class Command : public QObject
{
    Q_OBJECT

public:
    Command(QTcpSocket* socket, QDir dataDir, QObject* parent = nullptr) :
        QObject(parent), socket(socket), dataDir(dataDir) {}

    /*
     * Default destructor
     */
    virtual ~Command() {}

    /*
     * The method that runs when the command is called
     */
    virtual bool run(QString args) = 0;

    /*
     * Returns the name of the command
     */
    virtual QString getName() = 0;

protected:
    // TCP Socket the client is connected to
    QTcpSocket* socket;

    // Client file requests will be relative to this directory
    QDir dataDir;

};

#endif // COMMAND_H
