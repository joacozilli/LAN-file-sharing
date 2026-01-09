#include "../include/controllers.h"
#include <stdio.h>
#include <stddef.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>

/**
 * read len bytes from clientFd into buffer. 
 */
static int recv_all(int clientFd, u_int8_t* buffer, u_int32_t len) {
    int totalBytesRead = 0;
    int nbytes;
    
    while (totalBytesRead < len) {
        nbytes = recv(clientFd, buffer + totalBytesRead, len - totalBytesRead, 0);
        if (nbytes > 0)
            totalBytesRead += nbytes;
        else
            break;
    }
    if (nbytes <= 0)
        return nbytes;
}

connection_status handle_client_request(int clientFd) {
    u_int8_t buffer[1024];
}

// if no data has been transferred and the timeout has been reached, then -1 is returned with errno  set
//               to  EAGAIN  or EWOULDBLOCK, or EINPROGRESS

/*
if timeout reached, we consider two cases:
- client sent a message of length less than len (an invalid request), in which case the server ignores the message with no warning
- client sent a valid request but is taking too long to arrive, in which case the server does not wait for it and informs the client

*/