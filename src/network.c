#include "../include/network.h"

#include <stdio.h> /* perror */
#include <stdlib.h>
#include <unistd.h> /* close */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* htons */
#include <sys/epoll.h>
#include <sys/time.h>

#include <string.h> /* memset */

#include <utils.h>



int create_tcp_listener_socket(int port, const char* ip, unsigned int connectionLimit)
{
    if (connectionLimit < 1) {
        eprintf("%s: connection limit %d is non-positive\n",__func__, connectionLimit);
        return -1;
    }
    if (port < 1 || port > 65535) {
        eprintf("%s: invalid port %d\n", __func__, port);
        return -1;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        errnoprintf("socket in %s", __func__);
        return -1;
    }

    /**
     * force to use the specified address (except if there is already an active socket listening
     * in such address).
     */
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) < 0) {
        close(fd);
        errnoprintf("setsockopt in %s", __func__);
        return -1;
    }

    struct sockaddr_in sa;

    /* cleanse memory from any garbage */
    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);


    if (ip == NULL)
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
    else {
        /* the ip is given as a string, so it must be converted to binary network format */
        int ret = inet_pton(AF_INET, ip, &sa.sin_addr);
        if (ret != 1) {
            close (fd);
            if (ret == 0)
                eprintf("inet_pton in %s: invalid ip\n", __func__);
            else if (ret == -1)
                errnoprintf("inet_pton in %s", __func__);
            return -1;
        }
    }
    
    if (bind(fd, (struct sockaddr*) &sa, sizeof sa) < 0) {
        close(fd);
        errnoprintf("bind in %s", __func__);
        return -1;
    }

    if (listen(fd, connectionLimit) < 0) {
        close(fd);
        errnoprintf("listen in %s", __func__);
        return -1;
    }
    
    return fd;
}

int create_srv_epoll(int srvSock) {
    /**
     * the listening socket (server socket) must be added with EPOLLIN event (a file descriptor
     * with EPOLLIN event means epoll_wait will notify when it is ready for read operations).
     * When a client tries to connect, epoll_wait will notify it and thus the listener can
     * accept and add the client's file descriptor to the epoll instance.
     * 
     * listening sockets are considered writable by default (they are not used for sending
     * data, so the kernel treats them as ready for writing). Therefore, if the listening 
     * socket is added with EPOLLOUT event, epoll_wait will always notify it as ready
     * for write operations (which is useless).
     */

    int epfd = epoll_create1(0);
    if (epfd < 0) {
        errnoprintf("epoll_create1 in %s", __func__);
        return -1;
    }

    SocketInfo server = malloc(sizeof(struct _SocketInfo));
    if (!server) {
        close(epfd);
        errnoprintf("NULL return by malloc in %s", __func__);
        return -1;
    }

    server->fd = srvSock;
    server->type = SOCKET_TCP_LISTENER;
    struct epoll_event srvEvent;
    srvEvent.data.ptr = server;
    srvEvent.events = EPOLLIN | EPOLLONESHOT; // prevents several threads accept the same connection

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, srvSock, &srvEvent) < 0) {
        close(epfd);
        errnoprintf("epoll_ctl in %s", __func__);
        return -1;
    }

    return epfd;
}

int accept_client_connection(int epfd, int srvSock) {
    /* got no reason to store client address */
    int clientfd = accept(srvSock, NULL, NULL);

    if (clientfd < 0) {
        errnoprintf("accept in %s", __func__);
        return -1;
    }

    struct timeval tv;
    tv.tv_sec = RECV_TIMEOUT_SEC;
    tv.tv_usec = 0;

    if (setsockopt(clientfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv) < 0) {
        errnoprintf("setsockopt in %s", __func__);
        close(clientfd);
        return -1;
    }

    SocketInfo sockInf = malloc(sizeof(struct _SocketInfo));
    sockInf->fd = clientfd;
    sockInf->type = SOCKET_TCP_CLIENT;
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLONESHOT;
    event.data.ptr = sockInf;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &event) < 0) {
        free(sockInf);
        close(clientfd);
        errnoprintf("epoll_ctl in %s (unable to add client to epoll)", __func__);
        return -1;
    }
    return 0;
}


void wait_epoll_events(int epfd, connection_status (*handler)(int clientFd)) {
    struct epoll_event eventsQueue[MAX_EVENTS];
    int eventsReady = epoll_wait(epfd, eventsQueue, MAX_EVENTS, -1);
    if (eventsReady < 0) {
        errnoprintf("epoll_wait in %s", __func__);
        return -1;
    }
    for (int i = 0; i < eventsReady; i++) {
        SocketInfo socket = eventsQueue[i].data.ptr;

        if (socket->type  == SOCKET_TCP_LISTENER) {
            accept_client_connection(epfd, socket->type);
            struct epoll_event srvEvent;
            srvEvent.data.ptr = socket;
            srvEvent.events = EPOLLIN | EPOLLONESHOT;
            epoll_ctl(epfd, EPOLL_CTL_MOD, socket->fd, &srvEvent);
            continue;
        }

        /**
         * handle client and rearm to epoll.
         */
        connection_status status = handler(socket->fd);

        if (status == CONTINUE) {
            struct epoll_event cliEvent;
            cliEvent.data.ptr = socket;
            cliEvent.events = EPOLLIN | EPOLLONESHOT;
            
            /* if unable to add again to epoll, close connection (very rare though) */
            if (epoll_ctl(epfd, EPOLL_CTL_MOD, socket->fd, &cliEvent) < 0) {
                close(socket->fd);
                free(socket);
                errnoprintf("epoll_ctl in %s", __func__);
            }

        }
        else if (status == CLOSE)
            epoll_ctl(epfd, EPOLL_CTL_DEL, socket->fd, NULL);
            close(socket->fd);
            free(socket);
    }
}