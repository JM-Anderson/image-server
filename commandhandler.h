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
        commands[cmd->getName().toLower()] = cmd;
    }

    CommandHandler* operator <<(Command* cmd) {
        add(cmd);
        return this;
    }

    Command* getCmd(const QString cmdName) {
        if (commands.contains(cmdName.toLower())) {
            Command* cmd = commands.value(cmdName);
            return cmd;
        } else {
            cout << "Command not found: " << cmdName.toStdString() << "\n";
            return nullptr;
        }
    }
};

#endif // COMMANDHANDLER_H
