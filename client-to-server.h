#ifndef CLIENT_TO_SERVER_H
#define CLIENT_TO_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip6.h>

#include <chrono>
#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#include <cstring>
#include <string>

#include "types.h"

constexpr int CLIENT_MIN_SIZE = 13;
constexpr int CLIENT_MAX_SIZE = 33;

constexpr int READ_ERROR    = -1;
constexpr int READ_COMPLETE = -2;
constexpr int READ_INVALID  = -3;

using session_id_t = uint64_t;
using turn_direction_t = uint8_t;
using event_no_t = uint32_t;

constexpr turn_direction_t TURN_STRAIGHT = 0;
constexpr turn_direction_t TURN_LEFT = 1;
constexpr turn_direction_t TURN_RIGHT = 2;

typedef struct client_to_server {
    session_id_t session_id;
    turn_direction_t turn_direction;
    event_no_t next_expected_event_no;
    std::string player_name;
    
    sockaddr client_address;
    socklen_t client_addr_len;

    TimePoint req_time;
} cts_t;

constexpr int CLIENT_REQUEST_DELAY_MS = 30;

int read_from_client(int listen_sock, cts_t& req, bool noblock);

#endif /* CLIENT_TO_SERVER_H */