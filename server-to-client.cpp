#include "server-to-client.h"

int send_to_client_blank(int listen_sock, const sockaddr* address, socklen_t addr_len) {
    char* buf = "karamba";
    sendto(listen_sock, buf, 8, 0, address, addr_len);
}