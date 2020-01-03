#ifndef THREADINFO_H
#define THREADINFO_H

#include <QHostAddress>
#include <iostream>

#include "command.h"

using namespace std;

class ThreadInfo : public Command {
    Q_OBJECT

public:
    using Command::Command;

    QString getName() {
        return "current";
    }

    bool isBlocking() {
        return false;
    }

    bool run(QString arg) {
        QString msg = QString(
                    "Thread ID: %1 \n"
                    "Outgoing data connection: %2:%3\n")
                .arg(threadID).arg(outSocket->peerAddress().toString()).arg(outSocket->peerPort());
        cout << msg.toStdString();
        return true;
    }
};

#endif // THREADINFO_H
