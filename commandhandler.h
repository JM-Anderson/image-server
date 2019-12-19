#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <iostream>
#include <QMap>

#include "command.h"

using namespace std;

class CommandHandler {

private:
    QMap<QString, Command*> commands;

public:
    CommandHandler() {}
    ~CommandHandler() {
        qDeleteAll(commands);
        commands.clear();
    }

    void add(Command* cmd) {
        commands[cmd->getName()] = cmd;
    }

    CommandHandler* operator <<(Command* cmd) {
        add(cmd);
        return this;
    }

    bool tryRun(const QString cmdName, const QString args) {
        if (commands.contains(cmdName)) {
            cout << "[" << cmdName.toStdString() << "]\n";
            return commands.value(cmdName)->run(args);
        } else {
            cout << "Command not found: " << cmdName.toStdString() << "\n";
            return false;
        }
    }
};

#endif // COMMANDHANDLER_H
