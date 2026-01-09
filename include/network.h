#ifndef __NETWORK_H__
#define __NETWORK_H__

/**
 * max events returned by epoll_wait
 */
#define MAX_EVENTS 1000

/**
 * timeout for calling recv over a client socket (in seconds).
 */
#define RECV_TIMEOUT_SEC 5

typedef enum {
    SOCKET_TCP_LISTENER, 
    SOCKET_TCP_CLIENT
} socket_type;

/**
 * describes if client closes connection or continues after handling its request.
 */
typedef enum {
    CONTINUE,
    CLOSE
} connection_status;

struct _SocketInfo {
    int fd;
    socket_type type;
};
typedef struct _SocketInfo* SocketInfo;


/**
 * create new tcp socket listening on the specified port and ip. if ip is NULL, bind
 * to all interfaces. Return the file descriptor of the socket on success, otherwise 
 * return -1 and inform the error.
 */
int create_tcp_listener_socket(int port, const char* ip, unsigned int connectionLimit);

/**
 * create epoll instance and add the server socket to it. Return the epoll file descriptor
 * on success, otherwise return -1.
 */
int create_srv_epoll(int srvSock);

/**
 * accept the new connection and add the client to the epoll instance. Return 0 on success,
 * otherwise return -1.
 */
int accept_client_connection(int epfd, int srvSock);

/**
 * wait for epoll events. If the ready file descriptor is the server, a new connection is accepted;
 * if it is a client, it is passed to the handler function to handle its request.
 */
void wait_epoll_events(int epfd, connection_status (*handler)(int clientFd));


#endif /* __NETWORK_H__ */