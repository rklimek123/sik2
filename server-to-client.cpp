#include "server-to-client.h"

int send_to_client_blank(int listen_sock, const sockaddr* address, socklen_t addr_len) {
    sendto(listen_sock, NULL, 0, 0, address, addr_len);
}