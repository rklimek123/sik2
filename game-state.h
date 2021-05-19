#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include <unistd.h>

#include <iostream>
#include <queue>

#include "types.h"

enum EventType {
    NEW_GAME          = 0,
    PIXEL             = 1,
    PLAYER_ELIMINATED = 2,
    GAME_OVER         = 3
};

typedef struct event {
    EventType event_type;
    void* event_data;
} event_t;

typedef struct data_new_game {
    dimensions;
} data_new_game_t;


class GameState {
    public:
        GameState(int argc, char* argv[]) {
            parse_input_parameters(argc, argv);
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

        uint64_t game_id;
        std::queue<event_t> events;

        void parse_port(const char* num_str);
        void parse_seed(const char* num_str);
        void parse_turning_speed(const char* num_str);
        void parse_rounds_per_sec(const char* num_str);
        void parse_board_width(const char* num_str);
        void parse_board_height(const char* num_str);
        void parse_input_parameters(int argc, char* argv[]);
};

#endif /* GAME_STATE_H */