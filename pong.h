#ifndef PONG_H
#define PONG_H

#include "command.h"

class Pong : Command
{
public:
    void init() override;
    void run(QByteArray args) override;
};

#endif // PONG_H
