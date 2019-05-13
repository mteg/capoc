//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_TCPSERVER_H
#define CAPOC2_TCPSERVER_H

#ifndef DUMMY_NET
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


/* This describes an I/O buffer */
struct tcpConnBuffer
{
    unsigned int size;	/* Number of bytes kept under mem */
    unsigned int alloc;	/* Amount of memory allocated for this buffer (size <= alloc) */
    char *mem;		/* The actual data */
};

struct tcpConn
{
    struct tcpConn *next;	/* Next connection in the linked list */
    struct tcpServer *server;

    int fd;			/* Socket */

    unsigned char state;	/* Connection state - see below */

#ifndef DUMMY_NET
    struct sockaddr_in peer;	/* Peer IP address/port */
#endif

    struct tcpConnBuffer i;	/* Input buffer */
    struct tcpConnBuffer o;	/* Output buffer */

    void *user_data;

};

#define STATE_OPEN 0
#define STATE_CLOSING 1


class tcpServer {
public:
    tcpServer(int maxconn, int buffer_limit);

    int listening_socket;	/* File descriptor of the listening socket */
    int conn_limit;		/* Max # of simultaneous connections */
    unsigned int buff_limit;		/* Max amount of memory allocated for a single I/O buffer (1 connection = 2 buffers) */

    int conn_count;		/* Current number of connections */
    void *user_data;

#define SERVER_PRINTF_BUFFER_SIZE 2048
    char *printf_buffer;

    struct tcpConn * connections;	/* Linked list of connections */


    int bind(int port, const char *address);

    void assert(void *m);

    int doNetwork();

    int debug(const char *fmt, ...);

    virtual  int input(struct tcpConn *c, void *udata, char *buf, int len)  = 0 ;
    virtual void disconnect(struct tcpConn *c, void *udata) = 0;
    virtual void * connect(struct tcpConn *c) = 0;

#ifndef DUMMY_NET
    void updateFdSets(fd_set *rd, fd_set *wr, fd_set *ex, int *maxfd);
#endif

    void writeTo(struct tcpConn *c, const void *ptr, size_t len);

    void printfTo(tcpConn *c, char *fmt, ...);

    void writeAll(const void *ptr, size_t len);

    void drop(tcpConn *c);

    void emergencyShutdown();
};

#define ALLOC_STEP 8192
#define  SERVER_ALLOC_START 16384


#endif //CAPOC2_TCPSERVER_H
