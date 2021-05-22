#include "event-parser.h"

namespace {
    void parse_data_newgame(event_t& event, unsigned char** parsed_out, size_t* parsed_out_size) {
        size_t size = sizeof(event.maxx) + sizeof(event.maxy);

        for (const std::string& s: event.player_names) {
            size += s.size() + 1;
        }

        *parsed_out = (unsigned char *)malloc(size);
        if (*parsed_out == NULL) {
            throw new std::runtime_error("Not enough memory");
        }

        dimensions_t* data_as_dim = (dimensions_t*)*parsed_out;
        data_as_dim[0] = htonl(event.maxx);
        data_as_dim[1] = htonl(event.maxy);

        unsigned char* current_byte = (*parsed_out) + sizeof(event.maxx) + sizeof(event.maxy);

        for (const std::string& s: event.player_names) {
            for (const unsigned char c: s) {
                *(current_byte++) = c;
            }
            *(current_byte++) = '\0';
        }

        *parsed_out_size = size;
    }

    void parse_data_pixel(event_t& event, unsigned char** parsed_out, size_t* parsed_out_size) {
        size_t size = sizeof(event.player_number) + sizeof(event.x) + sizeof(event.y);

        *parsed_out = (unsigned char *)malloc(size);
        if (*parsed_out == NULL) {
            throw new std::runtime_error("Not enough memory");
        }

        (*parsed_out)[0] = event.player_number;

        dimensions_t* data_as_dim = (dimensions_t*)(*parsed_out + 1);
        data_as_dim[0] = htonl(event.x);
        data_as_dim[1] = htonl(event.y);

        *parsed_out_size = size;
    }

    void parse_data_player_eliminated(event_t& event, unsigned char** parsed_out, size_t* parsed_out_size) {
        size_t size = sizeof(event.player_number);

        *parsed_out = (unsigned char *)malloc(size);
        if (*parsed_out == NULL) {
            throw new std::runtime_error("Not enough memory");
        }

        (*parsed_out)[0] = event.player_number;
        *parsed_out_size = size;
    }

    void parse_data(event_t& event, unsigned char** parsed_out, size_t* parsed_out_size) {
        switch (event.event_type) {
            case EVENT_NEW_GAME:
                parse_data_newgame(event, parsed_out, parsed_out_size);
                break;
            
            case EVENT_PIXEL:
                parse_data_pixel(event, parsed_out, parsed_out_size);
                break;
            
            case EVENT_PLAYER_ELIMINATED:
                parse_data_player_eliminated(event, parsed_out, parsed_out_size);
                break;
            
            case EVENT_GAME_OVER:
                *parsed_out = NULL;
                *parsed_out_size = 0;
                break;
            
            default:
                throw new std::invalid_argument("Event type not supported");
                break;
        }
    }
}

void parse_event(event_t& event) {
    unsigned char* parsed_data;
    size_t parsed_data_size;

    parse_data(event, &parsed_data, &parsed_data_size);
    
    event.len = parsed_data_size + sizeof(event.event_type) + sizeof(event.event_no);
    
    event.sendable_size = sizeof(event.len) + event.len + sizeof(event.crc32);
    event.sendable = (unsigned char *)malloc(event.sendable_size);
    if (event.sendable == NULL) {
        throw new std::runtime_error("Not enough memory");
    }

    uint32_t* event32 = (uint32_t*)event.sendable;
    event32[0] = htonl(event.len);
    event32[1] = htonl(event.event_no);
    event.sendable[8] = event.event_type;
    
    for (size_t i = 0; i < parsed_data_size; ++i) {
        unsigned char c = parsed_data[i];
        event.sendable[9 + i] = c;
    }

    if (parsed_data_size != 0)
        free(parsed_data);

    event.crc32 = crc32(event.sendable, event.sendable_size - sizeof(event.crc32));
    uint32_t* insert_crc = (uint32_t*)(event.sendable + event.sendable_size - sizeof(event.crc32));
    insert_crc[0] = htonl(event.crc32);
}
