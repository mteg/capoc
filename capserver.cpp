/*
 *
 * Copyright (c) 2018 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#include "capserver.h"


int capServer::input(struct tcpConn *c, void *udata, char *buf, int len) {
    auto *ct = (capCliThread*) udata;
    ct->input((unsigned char*) buf, (unsigned int)  len);
    return len;
}
void capServer::disconnect(struct tcpConn *c, void *udata) {
    auto *ct = (capCliThread*) udata;
    delete ct;
}
void * capServer::connect(struct tcpConn *c) {
    auto *ct = new capCliThread(this->cliController, c);
    return ct;
}

capServer::capServer(int maxconn, int buffer_limit, capCli *c) : tcpServer(maxconn, buffer_limit) {
    this->cliController = c;
    c->server = this;
}
