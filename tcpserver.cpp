/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */


#include "tcpserver.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#define SERVER

#ifndef DUMMY_NET
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cstdarg>

#endif

#include "tcpserver.h"


int tcpServer::bind(int port, const char *address)
{
#ifndef DUMMY_NET
    struct sockaddr_in sin;
#endif
    int listeningSocket = 0;

#ifndef DUMMY_NET
    int on = 1;
    /* Acquire socket */
    if((listeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("socket()");
        return 0;
    }
    /* kill "Address already in use" error message */
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == -1)
    {
        perror("setsockopt(SO_REUSEADDR)");
        return 0;
    }
    /* Bind it to a port */
    sin.sin_family = PF_INET;
    sin.sin_port = htons((u_int16_t) port);
    if(address)
    {
        if(!inet_aton(address, &sin.sin_addr))
        {
            perror("inet_aton()");
            return 0;
        }
    }
    else
        sin.sin_addr.s_addr = INADDR_ANY;

    if(::bind(listeningSocket, (struct sockaddr*) &sin, sizeof(sin)) == -1)
    {
        perror("bind()");
        return 0;
    }

    /* Configure listening */
    if(listen(listeningSocket, 10) == -1)
    {
        perror("listen()");
        return 0;
    }

    /* Setup non-blocking operation */
    if(fcntl(listeningSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl(listeningSocket, F_SETFL, O_NONBLOCK)");
        return(0);
    }

    /* Initialize the server structure */
#endif

    this->listening_socket = listeningSocket;
	return 1;
}

void tcpServer::assert(void *m)
{
    if(!m) abort();
}

int tcpServer::debug(const char *fmt, ...) {
#ifdef DEBUG
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
#endif
    return 0;
}



tcpServer::tcpServer(int maxconn, int buffer_limit)
{
	this->conn_limit = maxconn;
	this->buff_limit = buffer_limit;

	this->assert((this->printf_buffer = (char*) malloc(SERVER_PRINTF_BUFFER_SIZE)));

	this->connections = NULL;
	this->conn_count = 0;
	this->user_data = user_data;
}

int tcpServer::doNetwork()
{
#ifndef DUMMY_NET
	struct sockaddr_in newSocketAddress;
	size_t addrlen;
	int sock;
	struct tcpConn *c, *next;
	struct tcpConn **prev;

	/* Loop through all connections and see if someone needs help ... */
	for(c = this->connections; c; c = c->next)
	{
		int we_read = 0;	/* Did we manage to get any new data? */

		if(c->o.size)	/* Does this connection have pending data to output? */
		{
			int wr_b;
			if((wr_b = (int) write(c->fd, c->o.mem, (size_t) c->o.size)) < 0)	/* Write data to socket */
			{
				if(errno != EAGAIN)	/* Write error, close the connection */
				{
					c->state = STATE_CLOSING;
					perror("write()");
					this->debug("### Network ### [%d] Write error, closing connection to %s:%d\n", c->fd,
							inet_ntoa(c->peer.sin_addr), ntohs(c->peer.sin_port));
				}
			}
			else if(wr_b > 0)	/* Succesfully written some bytes */
			{
				if((unsigned int) wr_b < c->o.size)
					/* Maybe this is not the most elegant thing to do, but
					   we expect that usually wr_b = c->o.size and the OS will take
					   care of further buffering. But if not - shift the buffer. */
					memmove(c->o.mem, ((unsigned char*) c->o.mem) + wr_b, c->o.size - wr_b);
				c->o.size -= wr_b;
			}
		}

		/* Now we'll try to see if there is new data available FROM this connection */
		while(true)
		{
			int rd_b;
			/* We'll be reading in chunks of maximally ALLOC_STEP bytes */

			/* So, if there is no more than ALLOC_STEP bytes free in the buffer,
			   then we must allocate more memory... */
			if((c->i.alloc - c->i.size) <  ALLOC_STEP)
			{
				char *new_mem;
				c->i.alloc += ALLOC_STEP;
				if(!(new_mem = (char*) realloc(c->i.mem, c->i.alloc)))
				{
					/* Not enough memory to raise the buffer */
					c->state = STATE_CLOSING;
					break;
				}
				else
					c->i.mem = new_mem;
			}

			/* Haven't we exceeded the permitted buffer size? */
			if(c->i.alloc > this->buff_limit)
			{
				debug("### Network ### [%d] Read buffer exceeded, closing connection to %s:%d\n", c->fd,
						inet_ntoa(c->peer.sin_addr), ntohs(c->peer.sin_port));
				c->state = STATE_CLOSING;
				break;
			}

			/* Try reading to the free space in the buffer. */
			rd_b = (int) read(c->fd, c->i.mem + c->i.size, ALLOC_STEP);
			if(rd_b < 0)
			{
				if(errno != EAGAIN)
				{
					c->state = STATE_CLOSING;
					perror("read()");
					debug("### Network ### [%d] Read error, closing connection to %s:%d\n", c->fd,
							inet_ntoa(c->peer.sin_addr), ntohs(c->peer.sin_port));
				}
				/* Maybe there's simply no data available */
				break;
			}

			if(rd_b == 0)	/* Peer closed the connection */
			{
				debug("### Network ### [%d] EOF, closing connection to %s:%d\n", c->fd,
						inet_ntoa(c->peer.sin_addr), ntohs(c->peer.sin_port));
				c->state = STATE_CLOSING;
				break;
			}

			/* Getting here means rd_b > 0, so we have new data! */
			c->i.size += rd_b;

			we_read = 1;
			/* We'll try reading the buffer again (remember we're in a loop)! */
		}

		if(we_read)
		{
			if(c->i.size)
			{
				int read = this->input(c, c->user_data, c->i.mem, c->i.size);
				if(read < (int) c->i.size)
					memmove(c->i.mem, c->i.mem + read, c->i.size - read);
				c->i.size -= read;
			}
		}
	}

	/* Now we'll loop through the connections again and really CLOSE the ones that we want to close. */
	prev = &this->connections;	/* Pointer to the place where we should UNLINK the current element */
	for(c = *prev; c; c = next)
	{
		next = c->next;	/* We must cache this because in a moment c->next might vanish */
		if(c->state == STATE_CLOSING)
		{
			debug("### Network ### [%d] Shutting down connection to %s:%d\n", c->fd,
							inet_ntoa(c->peer.sin_addr), ntohs(c->peer.sin_port));
			this->disconnect(c, c->user_data);
			close(c->fd);
			free(c->i.mem);
			free(c->o.mem);
			*prev = c->next;	/* Correct the next pointer in the previous element */
			free(c);
			this->conn_count --;	/* Decrease current connection count */
		}
		else
			prev = &c->next;	/* This connection is still valid,
						   but if the next one occurs to be scheduled for closing,
						   this is the place where it should be unlinked from */
	}

	/* Now check if there're any new connections */
	addrlen = sizeof(struct sockaddr_in);
	if((sock = accept(this->listening_socket, (struct sockaddr*) &newSocketAddress, (socklen_t*) &addrlen)) >= 0)
	{
		/* Set nonblock operation */
		if(fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
		{
			perror("fcntl(newSocket, F_SETFL, O_NONBLOCK)");
			close(sock);
		}
		else if(this->conn_count > this->conn_limit)	/* See if we meet the limit of simultaneous connections */
			close(sock);
		else
		{
			struct tcpConn * c;
			/* Allocate connection structure */
			if(!(c = (struct tcpConn*) malloc(sizeof(struct tcpConn))))
				/* Not enough memory */
				close(sock);
			else
			{
				memset(c, 0, sizeof(struct tcpConn));
				this->conn_count++;
				c->fd = sock;
				c->state = STATE_OPEN;	/* indicate that we're waiting for a request in this connection */
				c->user_data = this->connect(c);

				/* Copy the peer IP/port to the connection structure */
				memcpy(&c->peer, &newSocketAddress, sizeof(struct sockaddr_in));

				debug("### Network ### [%d] Accepted connection from %s:%d\n", sock,
							inet_ntoa(newSocketAddress.sin_addr), ntohs(newSocketAddress.sin_port));

				/* Link the connection to the connections list */
				c->next = this->connections;
				this->connections = c;
			}
		}
	}
	else if(errno != EAGAIN)
	{
		perror("accept()");
		return 1;	/* Trouble! */
	}
#endif
	return 0;
}

#ifndef DUMMY_NET
/* server_fdset - update fd_sets for select() */
void tcpServer::updateFdSets(fd_set *rd, fd_set *wr, fd_set *ex, int *maxfd)
{
	struct tcpConn *c;

	/* Add the listening socket */
	FD_SET(this->listening_socket, rd);

	/* Check if we need to increase the highest fd number */
	if(this->listening_socket > *maxfd)
		*maxfd = this->listening_socket;

	/* Loop through all connections and add their sockets to the set */
	for(c = this->connections; c; c = c->next)
	{
		if(c->fd > *maxfd)
			*maxfd = c->fd;
		FD_SET(c->fd, rd);	/* Add the socket to the read set, no matter what's happening */
		if(c->o.size)
			FD_SET(c->fd, wr);	/* Add the socket to the write set if there's some data pending
						   in the output buffer */
	}
}
#endif

/* server_write_conn() - append data to the connection output buffer */
void tcpServer::writeTo(struct tcpConn *c, const void *ptr, size_t len)
{
#ifndef DUMMY_NET
	/* Prepare more space if it will not fit... */
	while((c->o.size + len) > c->o.alloc)
	{
		char *new_mem;
		c->o.alloc *= 2;
		if(!c->o.alloc)
			c->o.alloc = SERVER_ALLOC_START;

		debug("### Network ### Memory: Raising allocation in output buffer for %d to %d bytes\n", c->fd, c->o.alloc);

		if(!(new_mem = (char*) realloc(c->o.mem, c->o.alloc)))
			/* Not enough memory */
			c->state = STATE_CLOSING;
		else
			c->o.mem = new_mem;

		/* The peer does not manage to receive the data ... */
		if(c->o.alloc > this->buff_limit)
		{
			debug("### Network ### [%d] Write buffer exceeded, closing connection to %s:%d\n", c->fd,
					inet_ntoa(c->peer.sin_addr), ntohs(c->peer.sin_port));
			c->state = STATE_CLOSING;
		}
	}
	if(c->state != STATE_CLOSING)
	{
		/* Copy the data into the connection output buffer */
		memcpy(c->o.mem + c->o.size, ptr, len);
		c->o.size += len;
	}
#endif
}

void tcpServer::printfTo(struct tcpConn *c, char * fmt, ...)
{
	int len;
	va_list app;
	va_start(app, fmt);
	len = vsnprintf(this->printf_buffer, SERVER_PRINTF_BUFFER_SIZE - 16, fmt, app);
	if(len > 0)
		this->writeTo(c, this->printf_buffer, len);
}


/* server_write() - append data to output buffers of all connections */
void tcpServer::writeAll(const void *ptr, size_t len)
{
	struct tcpConn *c;

	for(c = this->connections; c; c = c->next)
		this->writeTo(c, ptr, len);
}

void tcpServer::drop(struct tcpConn *c)
{
	c->state = STATE_CLOSING;
}

void tcpServer::emergencyShutdown()
{
#ifndef DUMMY_NET
	struct tcpConn *c;
	for(c = this->connections; c; c = c->next)
		close(c->fd);
	close(this->listening_socket);
#endif
}
