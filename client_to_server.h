#ifndef CLIENT_TO_SERVER_H
#define CLIENT_TO_SERVER_H

#include <endian.h>
#include <netinet/in.h>
#include <stdint.h>

#include <string>

typedef struct client_to_server {
    uint64_t session_id;
    uint8_t turn_direction;
    uint32_t next_expected_event_no;
    std::string player_name;
} cts_t;

// data needs to be null terminated
cts_t parse_from_client_request(char* data) {
    uint64_t nsession_id = ((uint64_t*)data)[0];
    data += 8;

    uint8_t nturn_direction = ((uint8_t*)data)[0];
    data += 1;

    uint32_t nnext_expected_event_no = ((uint32_t*)data)[0];
    data += 4;

    cts_t result;
    result.session_id = be64toh(nsession_id);
    result.turn_direction = nturn_direction;
    result.next_expected_event_no = ntohl(nnext_expected_event_no);
    result.player_name = data;

    return result;
}

#endif /* CLIENT_TO_SERVER_H */