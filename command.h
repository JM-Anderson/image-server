#ifndef COMMAND_H
#define COMMAND_H

#include <QTcpSocket>
#include <QDir>

class Command : public QObject
{
    Q_OBJECT

public:
    Command(QTcpSocket* outSocket, QTcpSocket* inSocket, QDir dataDir, uint threadID, QObject* parent = nullptr) :
        QObject(parent), outSocket(outSocket), inSocket(inSocket), dataDir(dataDir), threadID(threadID) {}

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

    /*
     * Indicates whether this command may block for a long period of time.
     * This is used to determine whether the user should be switched to control
     * a different thread.
     */
    virtual bool isBlocking() = 0;

    int getRequestedThread() {
        return requestedThread;
    }

    virtual void reset() {
        requestedThread = -1;
    }

protected:
    // TCP Socket data is being sent to
    QTcpSocket* outSocket;

    // TCP Socket data is received from
    QTcpSocket* inSocket;

    // Client file requests will be relative to this directory
    QDir dataDir;

    // Threads
    const uint threadID;

    int requestedThread = -1;

};

#endif // COMMAND_H
