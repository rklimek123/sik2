#ifndef SERVER_TO_CLIENT_H
#define SERVER_TO_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip6.h>

#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#include <cstring>
#include <string>

typedef struct server_to_client {
    
} stc_t;

int send_to_client_blank(int listen_sock, const sockaddr* address, socklen_t addr_len);

#endif /* SERVER_TO_CLIENT_H */