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
        GameState(int argc, char* argv[]) {
            parse_input_parameters(argc, argv);
            rng = Random(seed);
        }

        void new_game();
    
    private:
        static constexpr const char* program_arguments = "p:s:t:v:w:h:";

        port_t port                     = DEFAULT_PORT;
        seed_t seed                     = DEFAULT_SEED;
        turn_speed_t turning_speed      = DEFAULT_TURNING_SPEED;
        rounds_per_sec_t rounds_per_sec = DEFAULT_ROUNDS_PER_SEC;
        dimensions_t board_width        = DEFAULT_BOARD_WIDTH;
        dimensions_t board_height       = DEFAULT_BOARD_HEIGHT;

        bool launched = false;
        int64_t round_number;

        uint64_t game_id;
        std::queue<event_t> events;
        std::map<std::string, player_number_t> players;

        Random rng;

        void parse_port(const char* num_str);
        void parse_seed(const char* num_str);
        void parse_turning_speed(const char* num_str);
        void parse_rounds_per_sec(const char* num_str);
        void parse_board_width(const char* num_str);
        void parse_board_height(const char* num_str);
        void parse_input_parameters(int argc, char* argv[]);
};

#endif /* GAME_STATE_H */
