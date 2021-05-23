#include "connection-manager.h"

namespace {
    constexpr int VERIFY_REPLACE = -1;
    constexpr int VERIFY_OK      = 0;
    constexpr int VERIFY_IGNORE  = 1;

    std::vector<std::pair<char*, size_t>>
    get_chopped_coms(const GameState& game, event_no_t begin_event, event_no_t end_event) {
        std::vector<std::pair<char*, size_t>> result;
        uint32_t game_id = htonl(game.get_game_id());
        
        event_no_t current_event = begin_event;
        char* chopped = (char*)malloc(MAX_BYTES_IN_STC_COMM);
        if (!chopped) throw new std::runtime_error("Buffer malloc error");
        size_t chopped_size = 0;

        uint32_t* chopped32 = (uint32_t*)chopped;
        chopped32[0] = game_id;
        chopped_size += sizeof(uint32_t);

        while (current_event <= end_event) {
            void* event_content;
            size_t event_size = game.get_event_at(current_event++, &event_content);

            if (chopped_size + event_size > MAX_BYTES_IN_STC_COMM) {
                result.push_back(std::make_pair(chopped, chopped_size));
                chopped = (char*)malloc(MAX_BYTES_IN_STC_COMM);
                if (!chopped) throw new std::runtime_error("Buffer malloc error");
                chopped_size = 0;

                chopped32 = (uint32_t*)chopped;
                chopped32[0] = game_id;
                chopped_size += sizeof(uint32_t);
            }

            char* event_bytes = (char*)event_content;
            for (size_t i = 0; i < event_size; ++i) {
                chopped[chopped_size++] = event_bytes[i];
            }
        }

        result.push_back(std::make_pair(chopped, chopped_size));
    }
}

player_number_t ConnectionManager::connected_players_count() const {
    return connected_players;
}

int ConnectionManager::index_ingame(const sockaddr& addr) const {
    int result = -1;
    auto it = connections.find(addr);
    if (it == connections.end()) {
        result = -1;
    }
    else {
        if (it->second.in_game) {
            result = it->second.player_number;
        }
    }

    return result;
}

void ConnectionManager::broadcast(int sock, GameState& game, event_no_t begin_event, event_no_t end_event) {
    end_event = std::min(game.event_log_len() - 1, end_event);

    if (begin_event <= end_event) {
        std::vector<std::pair<char*, size_t>> coms = get_chopped_coms(game, begin_event, end_event);
        for (const std::pair<char*, size_t>& com: coms) {
            for (map_iter it = connections.begin(); it != connections.end(); ++it) {
                size_t ret =
                    sendto(sock, com.first, com.second, 0, &(it->first), it->second.addr_len);
                
                if (ret != com.second)
                    throw new std::runtime_error("Sendto error");
            }

            free(com.first);
        }
    }
}

void ConnectionManager::check_activity() {
    TimePoint now = Clock::now();

    for (auto it = connections.begin(); it != connections.end();) {
        if (delay_ms(now, it->second.last_activity) >= CLIENT_TIMEOUT_MS) {
            remove_connection(it++);
        }
        else {
            ++it;
        }
    }
}

void ConnectionManager::attempt_client_reply(int sock,
                                             const cts_t& req,
                                             const GameState& game) {
    map_iter it = connections.find(req.client_address);
    if (it != connections.end()) {
        const con_state_t& con_stat = it->second;
        int ret = verify_connection(con_stat, req);
        if (ret == VERIFY_REPLACE) {
            remove_connection(it);
            add_connection(req);
        }
        if (ret != VERIFY_IGNORE) {
            event_no_t begin_event = req.next_expected_event_no;
            event_no_t end_event = game.event_log_len() - 1;

            if (begin_event <= end_event) {
                std::vector<std::pair<char*, size_t>> coms =
                    get_chopped_coms(game, begin_event, end_event);
        
                for (const std::pair<char*, size_t>& com: coms) {
                    size_t ret =
                        sendto(sock, com.first, com.second, 0, &(it->first), it->second.addr_len);
                        
                    if (ret != com.second)
                        throw new std::runtime_error("Sendto error");

                    free(com.first);
                }
            }
            else {
                send_to_client_blank(sock, &req.client_address, req.client_addr_len);
            }
        }
    }
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

void ConnectionManager::remove_connection(map_iter con) {
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
        if (connected_clients >= CLIENT_MAX_AMOUNT &&
            verify_unique_playername(req.player_name)) {
            
            add_connection(req);
        }
    }
    else {
        con_state_t& con_st = con->second;
        int ret = verify_connection(con_st, req);

        switch (ret) {
            case VERIFY_OK:
                con_st.last_activity = req.req_time;
                try_toggle_ready_state(con, req);
                break;
            
            case VERIFY_REPLACE:
                remove_connection(con);
                if (verify_unique_playername(req.player_name)) {
                    add_connection(req);
                }
                break;

            default:
                break;
        }
    }

    return connected_players >= 2 && ready_players == connected_players;
}

void ConnectionManager::handle_request_game(
    GameState& game, const cts_t& req, player_number_t player_index) {

    const sockaddr& addr = req.client_address;
    
    map_iter con = connections.find(addr);
    if (con != connections.end()) {
        throw new std::runtime_error("Handle request game invalid connection");
    }
    else {
        game.try_change_turning(player_index, req.turn_direction);
    }
}

player_number_t ConnectionManager::get_player_index(const std::string& playername) const {
    const auto& it = playernames.find(playername);
    if (it == playernames.end()) {
        throw new std::invalid_argument("No player of that name found");
    }

    return std::distance(playernames.begin(), it);
}

void ConnectionManager::prepare_for_new_game() {
    for (auto& con: connections) {
        con_state_t& con_st = con.second;

        if (con_st.is_player) {
            con_st.player_number = get_player_index(con_st.player_name);
            con_st.in_game = true;
            con_st.wants_to_play = false;
        }
    }

    ready_players = 0;
}

int ConnectionManager::send_to_client_blank(int listen_sock,
                         const sockaddr* address,
                         socklen_t addr_len) {
    
    if (connections.find(*address) != connections.end())
        sendto(listen_sock, NULL, 0, 0, address, addr_len);
}
