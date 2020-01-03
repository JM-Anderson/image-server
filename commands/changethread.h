#ifndef CHANGETHREAD_H
#define CHANGETHREAD_H

#include "command.h"

#include <iostream>

using namespace std;

class ChangeThread : public Command {
    Q_OBJECT

public:
    using Command::Command;

    QString getName() {
        return "select";
    }

    bool isBlocking() {
        return false;
    }

    bool run(QString arg) {
        bool *ok = new bool;
        int argInt = arg.toInt(ok);
        if (ok) {
            requestedThread = argInt;
            return true;
        } else {
            cout << "Format: select <threadID>\n";
            return false;
        }
    }

};

#endif // CHANGETHREAD_H
