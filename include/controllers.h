#ifndef __CONTROLLERS_H__
#define _CONTROLLERS_H__

#include "../include/network.h"

/**
 * main handler for client's requests.
 */
connection_status handle_client_request(int clientFd);

#endif /* _CONTROLLERS_H__ */