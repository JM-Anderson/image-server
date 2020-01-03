#ifndef SERVER_H
#define SERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QDir>

#include "thread.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QDir dataDir, QObject *parent = nullptr);
    uint getSelectedThread() {
        return selectedThread;
    }

private:
    QDir dataDir;

    // Threads
    QMap<uint, Thread*> threadPool;
    uint threadCount = 0;
    uint selectedThread = 0;

private slots:
    void selectThread(uint threadID);
    void selectAnyThread(uint threadID);
    void deleteThread(uint threadID);
    void threadFree(uint threadID);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif
