#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "err.h"
#include "event-parser.h"
#include "random.h"
#include "types.h"


struct Player {
    Player(double x_, double y_, short oriented_):
        x(x_), y(y_), oriented(oriented_), is_dead(false) {};

    double x;
    double y;
    short oriented;
    bool is_dead;
};


class GameState {
    public:
        GameState(Random& rng_,
                  turn_speed_t turning_speed_,
                  dimensions_t board_width_,
                  dimensions_t board_height_,
                  player_number_t players_):
            rng(rng_),
            turning_speed(turning_speed_),
            board_width(board_width_),
            board_height(board_height_),
            number_of_players(players_),
            alive_players(players_)
        {
            game_id = rng.rand();

            for (dimensions_t x = 0; x < board_width; ++x) {
                std::vector<bool> collumn;
                board.push_back(collumn);

                for (dimensions_t y = 0; y < board_height; ++y) {
                    board[x].push_back(false);
                }
            }
        }

        event_no_t event_log_len() const;
        event_no_t event_start_newgame(const std::set<std::string>& playernames);
        uint32_t get_game_id() const;
        size_t get_event_at(event_no_t index, void** out);
        event_no_t next_turn();
    
    private:
        std::vector<std::vector<bool>> board;

        turn_speed_t turning_speed;
        dimensions_t board_width;
        dimensions_t board_height;

        player_number_t number_of_players;
        player_number_t alive_players;

        std::vector<Player> players;

        uint32_t game_id;
        event_no_t next_event_no = 0;
        std::vector<event_t> events;

        Random& rng;

        void eat_pixel(player_number_t player, dimensions_t x, dimensions_t y);
        
        void generate_newgame_event(const std::set<std::string>& playernames);
        void generate_pixel_event(player_number_t player, dimensions_t x, dimensions_t y);
        void generate_player_eliminated_event(player_number_t player);
        void generate_gameover_event();
};

#endif /* GAME_STATE_H */
