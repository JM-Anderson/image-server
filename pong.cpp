#include "pong.h"

void Pong::init() {
    cmdByte = 'p';
}

void Pong::run(QByteArray args) {
    socket->write("pong");
    socket->waitForBytesWritten();
}
