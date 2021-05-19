#ifndef CLIENT_TO_SERVER_H
#define CLIENT_TO_SERVER_H

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

constexpr int CLIENT_MIN_SIZE = 13;
constexpr int CLIENT_MAX_SIZE = 33;

typedef struct client_to_server {
    uint64_t session_id;
    uint8_t turn_direction;
    uint32_t next_expected_event_no;
    std::string player_name;
} cts_t;

int read_from_client(int listen_sock, cts_t& req);

#endif /* CLIENT_TO_SERVER_H */