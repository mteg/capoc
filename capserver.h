//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CAPSERVER_H
#define CAPOC2_CAPSERVER_H

class capServer;

#include "tcpserver.h"
#include "capcli.h"

class capServer : public tcpServer {
public:
    capCli *cliController;
    capServer(int maxconn, int buffer_limit, capCli *c);

    int input(struct tcpConn *c, void *udata, char *buf, int len) final;
    void disconnect(struct tcpConn *c, void *udata) final;
    void * connect(struct tcpConn *c) final;


};


#endif //CAPOC2_CAPSERVER_H
