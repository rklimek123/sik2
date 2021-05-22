#ifndef EVENT_PARSER_H
#define EVENT_PARSER_H

#include <netinet/in.h>

#include <stdint.h>
#include <unistd.h>

#include <cstdlib>
#include <stdexcept>
#include <set>
#include <string>

#include "crc32.h"
#include "types.h"

constexpr uint8_t EVENT_NEW_GAME          = 0;
constexpr uint8_t EVENT_PIXEL             = 1;
constexpr uint8_t EVENT_PLAYER_ELIMINATED = 2;
constexpr uint8_t EVENT_GAME_OVER         = 3;

using event_no_t = uint32_t;

struct event_t {
    uint32_t len = 0;
    event_no_t event_no = 0;
    uint8_t event_type = 0;

    dimensions_t maxx = 0;
    dimensions_t maxy = 0;
    std::set<std::string> player_names;

    player_number_t player_number = 0;
    dimensions_t x = 0;
    dimensions_t y = 0;

    uint32_t crc32 = 0;
    unsigned char* sendable = NULL;
    size_t sendable_size = 0;

    ~event_t() {
        free(sendable);
    }
};

void parse_event(event_t& event);

#endif /* EVENT_PARSER_H */