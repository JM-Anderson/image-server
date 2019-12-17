#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <iostream>
#include <QMap>

#include "command.h"

using namespace std;

class CommandHandler {
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

    bool tryRun(const QString cmdName, const QByteArray args) {
        if (commands.contains(cmdName)) {
            cout << "Command not found: " << cmdName.toStdString();
            return commands.value(cmdName)->run(QByteArray());
        } else
            return false;
    }

private:
    QMap<QString, Command*> commands;
};

#endif // COMMANDHANDLER_H
