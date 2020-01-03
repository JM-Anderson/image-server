#include "server.h"

Server::Server(QDir dataDir, QObject *parent) : QTcpServer(parent), dataDir(dataDir)
{
}

void Server::deleteThread(uint threadID) {
    if (!threadPool.contains(threadID)) {
        return;
    }
    threadPool.remove(threadID);
}

/*
 * Switches control to the specified thread
 */
void Server::selectThread(uint threadID) {
    if (threadPool.contains(threadID)) {
        if (threadPool.value(threadID)->isBlocking()) {
            cout << "That thread is currently blocking\n";
        } else {
            selectedThread = threadID;
        }
    } else {
        cout << "Not a valid thread\n";
    }

    threadPool.value(selectedThread)->disabled = false;
}

/*
 * Switches control to any thread that is not the specified thread
 * This is used when a thread starts blocking and thus the user should be
 * switched to control a different client
 */
void Server::selectAnyThread(uint threadID) {
    for (Thread* tr : threadPool) {
        if (!tr->isBlocking() && tr->threadID != threadID) {
            selectThread(tr->threadID);
        }
    }
}

/*
 * This handles when a thread becomes available (nonblocking)
 */
void Server::threadFree(uint threadID) {
    // If the current thread is blocking, switch to the new one
    if (threadPool.value(selectedThread)->isBlocking()) {
        cout << "Switching to thread " << threadID << "\n";
        selectThread(threadID);
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Thread *thread = new Thread(socketDescriptor, dataDir, threadCount, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, &Thread::requestChangeThread, this, &Server::selectThread);
    connect(thread, &Thread::clientDisconnected, this, &Server::deleteThread);
    connect(thread, &Thread::startedBlocking, this, &Server::selectAnyThread);
    connect(thread, &Thread::finishedBlocking, this, &Server::threadFree);

    threadPool.insert(threadCount, thread);

    if (threadCount == 0) {
        thread->disabled = false;
    }

    threadCount++;

    thread->start();

    threadFree(thread->threadID);
}
