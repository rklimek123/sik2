#include "connection-manager.h"

namespace {
    constexpr int VERIFY_REPLACE = -1;
    constexpr int VERIFY_OK      = 0;
    constexpr int VERIFY_IGNORE  = 1;
}

void ConnectionManager::add_connection(const cts_t& req) {
    con_state_t new_con;
    new_con.session_id = req.session_id;
    new_con.last_activity = req.req_time;
    new_con.player_name = req.player_name;
    new_con.player_number = 0;

    new_con.in_game = false;
    new_con.wants_to_play = req.turn_direction != TURN_STRAIGHT;
    new_con.is_player = !req.player_name.empty();

    new_con.addr_len = req.client_addr_len;

    auto result = connections.insert(std::make_pair(req.client_address, new_con));
    ++connected_clients;

    if (new_con.is_player) {
        ++connected_players;
        playernames.insert(req.player_name);

        if (new_con.wants_to_play) {
            ++ready_players;
        }
    }
}

void ConnectionManager::remove_connection(map_iter& con) {
    con_state_t& con_st = con->second;

    if (con_st.is_player) {
        playernames.erase(con_st.player_name);
        if (con_st.wants_to_play)
            --ready_players;
        --connected_players;
    }

    connections.erase(con);
    --connected_clients;
}

bool ConnectionManager::try_toggle_ready_state(map_iter& con, const cts_t& req) {
    bool toggled = false;
    con_state_t& con_st = con->second;

    if (con_st.is_player &&
        !con_st.wants_to_play &&
        !(req.turn_direction == TURN_STRAIGHT)) {

        con_st.wants_to_play = true;
        ++ready_players;
        toggled = true;
    }

    return toggled;
}

int ConnectionManager::verify_connection(const con_state_t& con, const cts_t& req) const {
    int result = VERIFY_IGNORE;
    
    if (con.session_id > req.session_id) {
        result = VERIFY_IGNORE;
    }
    else if (con.session_id == req.session_id) {
        if (con.player_name == req.player_name) {
            result = VERIFY_OK;
        }
        else {
            result = VERIFY_IGNORE;
        }
    }
    else {
        result = VERIFY_REPLACE;
    }

    return result;
}

bool ConnectionManager::verify_unique_playername(const std::string& playername) const {
    return playername.empty() || playernames.find(playername) == playernames.end();
}

bool ConnectionManager::handle_request_nogame(const cts_t& req) {
    const sockaddr& addr = req.client_address;

    map_iter con = connections.find(addr);
    if (con != connections.end()) {
        if (verify_unique_playername(req.player_name)) {
            add_connection(req);
        }
    }
    else {
        con_state_t& con_st = con->second;
        int ret = verify_connection(con_st, req);

        switch (ret) {
            case VERIFY_OK:
                try_toggle_ready_state(con, req);
                break;
            
            case VERIFY_REPLACE:
                remove_connection(con);
                add_connection(req);
                break;

            default:
                break;
        }
    }

    return connected_players >= 2 && ready_players == connected_players;
}
