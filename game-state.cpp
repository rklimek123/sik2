#include "game-state.h"

void Player::move() {
    double angle = (double)oriented * M_PI / 180;
    double delta_x = cos(angle);
    double delta_y = -sin(angle);

    x += delta_x;
    y += delta_y;
}


event_no_t GameState::event_log_len() const {
    return next_event_no;
}

void GameState::generate_newgame_event(const std::set<std::string>& playernames) {
    event_t ev;

    ev.maxx = board_width;
    ev.maxy = board_height;
    ev.player_names = playernames;
    
    ev.event_no = next_event_no++;
    ev.event_type = EVENT_NEW_GAME;

    parse_event(ev);
    events.push_back(ev);
}

void GameState::generate_pixel_event(player_number_t player, dimensions_t x, dimensions_t y) {
    event_t ev;

    ev.player_number = player;
    ev.x = x;
    ev.y = y;

    ev.event_no = next_event_no++;
    ev.event_type = EVENT_PIXEL;
    
    parse_event(ev);
    events.push_back(ev);
}

void GameState::generate_player_eliminated_event(player_number_t player) {
    event_t ev;
    
    ev.player_number = player;

    ev.event_no = next_event_no++;
    ev.event_type = EVENT_PLAYER_ELIMINATED;

    parse_event(ev);
    events.push_back(ev);
}

void GameState::generate_gameover_event() {
    event_t ev;
    ev.event_no = next_event_no++;
    ev.event_type = EVENT_GAME_OVER;

    parse_event(ev);
    events.push_back(ev);
}

void GameState::eat_pixel(player_number_t player, dimensions_t x, dimensions_t y) {
    if (x < 0 || x >= board_width || y < 0 || y >= board_height || board[x][y]) {
        players[player].is_dead = true;
        generate_player_eliminated_event(player);
        --alive_players;
    }
    else {
        board[x][y] = true;
        generate_pixel_event(player, x, y);
    }
}

event_no_t GameState::event_start_newgame(const std::set<std::string>& playernames) {
    generate_newgame_event(playernames);
    for (player_number_t i = 0; i < number_of_players; ++i) {
        double x = (double)(rng.rand() % board_width) + 0.5;
        double y = (double)(rng.rand() % board_height) + 0.5;

        Player p(x, y, rng.rand() % 360);
        eat_pixel(i, (dimensions_t)x, (dimensions_t)y);
    }

    return next_event_no - 1;
}

uint32_t GameState::get_game_id() const {
    return game_id;
}

size_t GameState::get_event_at(event_no_t index, void** out) {
    *out = events[index].sendable;
    return events[index].sendable_size;
}

event_no_t GameState::next_turn() {
    for (player_number_t i = 0; i < number_of_players; ++i) {
        Player& player = players[i];

        switch (player.last_turn_direction) {
            case TURN_LEFT:
                player.oriented = (player.oriented + turning_speed) % 360;
                break;
            case TURN_RIGHT:
                player.oriented = (player.oriented - turning_speed) % 360;
                break;
            default:
                break;
        }
        
        dimensions_t previous_x = (dimensions_t)player.x;
        dimensions_t previous_y = (dimensions_t)player.y;
        player.move();

        if (previous_x != (dimensions_t)player.x || previous_y != (dimensions_t)player.y) {
            eat_pixel(i, player.x, player.y);
        }

        if (alive_players <= 1) {
            generate_gameover_event();
            break;
        }
    }

    return next_event_no - 1;
}

bool GameState::try_change_turning(player_number_t player_index,
                                   turn_direction_t turn_direction) {
    bool result = false;
    if (player_index >= number_of_players ||
        players[player_index].is_dead) {
        
        result = false;
    }
    else {
        players[player_index].last_turn_direction = turn_direction;
        result = true;
    }

    return result;
}

bool GameState::has_finished() const {
    return has_ended;
}
