#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "random.h"
#include "types.h"

constexpr uint8_t EVENT_NEW_GAME          = 0;
constexpr uint8_t EVENT_PIXEL             = 1;
constexpr uint8_t EVENT_PLAYER_ELIMINATED = 2;
constexpr uint8_t EVENT_GAME_OVER         = 3;

typedef struct event {
    uint32_t len;
    uint32_t event_no;
    uint8_t event_type;
    void* event_data;
    uint32_t crc32;
} event_t;

typedef struct data_new_game {
    dimensions_t maxx;
    dimensions_t maxy;
    std::vector<std::string> player_names;
} data_new_game_t;

typedef struct data_pixel {
    player_number_t player_number;
    dimensions_t x;
    dimensions_t y;
} data_pixel_t;

typedef struct data_player_eliminated {
    player_number_t player_number;
} data_player_eliminated_t;


class GameState {
    public:
        GameState(seed_t seed_,
                  turn_speed_t turning_speed_,
                  dimensions_t board_width_,
                  dimensions_t board_height_):
            seed(seed_),
            turning_speed(turning_speed_),
            board_width(board_width_),
            board_height(board_height_) {
            
            rng = Random(seed);
        }

        void new_game();
    
    private:
        seed_t seed;
        turn_speed_t turning_speed;
        dimensions_t board_width;
        dimensions_t board_height;

        bool launched = false;
        int64_t round_number;

        uint64_t game_id;
        std::vector<event_t> events;

        Random rng;
};

#endif /* GAME_STATE_H */
