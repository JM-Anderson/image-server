#ifndef COMMAND_H
#define COMMAND_H

#include <QTcpSocket>
#include <QDir>

class Command : public QObject
{
    Q_OBJECT

public:
    Command(QTcpSocket* outSocket, QTcpSocket* inSocket, QDir dataDir, QObject* parent = nullptr) :
        QObject(parent), outSocket(outSocket), inSocket(inSocket), dataDir(dataDir) {}

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
    // TCP Socket data is being sent to
    QTcpSocket* outSocket;

    // TCP Socket data is received from
    QTcpSocket* inSocket;

    // Client file requests will be relative to this directory
    QDir dataDir;

};

#endif // COMMAND_H
