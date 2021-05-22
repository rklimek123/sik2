#include "client-to-server.h"

namespace {
    bool verify_cts(cts_t& cts) {
        if (cts.turn_direction < 0 || cts.turn_direction > 2)
            return false;
        
        for (const char& c: cts.player_name) {
            if (c < 33 || c > 126)
                return false;
        }

        return true;
    }

    bool parse_from_client_request(char* data, cts_t& result) {
        uint64_t nsession_id = ((uint64_t*)data)[0];
        data += 8;

        uint8_t nturn_direction = ((uint8_t*)data)[0];
        data += 1;

        uint32_t nnext_expected_event_no = ((uint32_t*)data)[0];
        data += 4;

        result.session_id = be64toh(nsession_id);
        result.turn_direction = nturn_direction;
        result.next_expected_event_no = ntohl(nnext_expected_event_no);
        result.player_name = data;

        return verify_cts(result);
    }
}

int read_from_client(int listen_sock, cts_t& req, bool noblock) {
    char buffer[CLIENT_MAX_SIZE + 1];
    memset(buffer, 0, CLIENT_MAX_SIZE + 1);

    int ret = recvfrom(listen_sock, buffer, CLIENT_MAX_SIZE, 0, &(req.client_address), &(req.client_addr_len));
    if (ret == -1) {
        if (noblock && (errno == EAGAIN || errno == EWOULDBLOCK))
            return READ_COMPLETE;
        else
            return READ_ERROR;
    }
    else if (ret < CLIENT_MIN_SIZE) {
        return READ_INVALID;
    }

    if (parse_from_client_request(buffer, req))
        return ret;
    else
        return READ_INVALID;
}
