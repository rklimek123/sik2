#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <sys/socket.h>
#include <stdint.h>

#include <chrono>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <set>
#include <string>
#include <utility>

#include "client-to-server.h"
#include "types.h"

constexpr int CLIENT_TIMEOUT_MS = 2000;
constexpr unsigned int CLIENT_MAX_AMOUNT = 25;

struct con_state_t {
    session_id_t session_id;
    TimePoint last_activity;
    std::string player_name;
    player_number_t player_number;
    
    bool in_game;
    bool wants_to_play;
    bool is_player;

    socklen_t addr_len;
};

namespace sockaddr_compare {
    bool comp_addr6(const sockaddr_in6& a, const sockaddr_in6& b) {
        bool result = false;

        uint32_t a_addr[] = a.sin6_addr.s6_addr32;
        uint32_t b_addr[] = b.sin6_addr.s6_addr32;

        for (int i = 0; i < 4; ++i) {
            if (a_addr[i] < b_addr[i]) {
                result = true;
                break;
            }
            else if (a_addr[i] > b_addr[i]) {
                result = false;
                break;
            }
        }

        return result;
    }

    struct sockaddr_cmp {
        bool operator()(const sockaddr& a, const sockaddr& b) const {
            bool result = false;
            if (a.sa_family == AF_INET && b.sa_family == AF_INET6) {
                result = true;
            }
            else if (a.sa_family == AF_INET6 && b.sa_family == AF_INET) {
                result = false;
            }
            else if (a.sa_family == AF_INET && b.sa_family == AF_INET) {
                sockaddr_in* a_in = (sockaddr_in*)&a;
                sockaddr_in* b_in = (sockaddr_in*)&b;

                if (a_in->sin_addr.s_addr == b_in->sin_addr.s_addr) {
                    result = a_in->sin_port < b_in->sin_port;
                }
                else {
                    result = a_in->sin_addr.s_addr < b_in->sin_addr.s_addr;
                }
            }
            else if (a.sa_family == AF_INET6 && b.sa_family == AF_INET6) {
                sockaddr_in6* a_in = (sockaddr_in6*)&a;
                sockaddr_in6* b_in = (sockaddr_in6*)&b;

                bool a_less = comp_addr6(*a_in, *b_in);

                if (!a_less) {
                    bool b_less = comp_addr6(*b_in, *a_in);

                    if (!b_less) {
                        result = a_in->sin6_port < b_in->sin6_port;
                    }
                    else {
                        result = false;
                    }
                }
                else {
                    result = true;
                }
            }

            return result;
        }
    };
}

class ConnectionManager {
    public:
        using sockaddr_cmp = sockaddr_compare::sockaddr_cmp;
        using map_iter = std::map<sockaddr, con_state_t, sockaddr_cmp>::iterator;

        ConnectionManager();

        void check_activity();
        bool handle_request_nogame(const cts_t& req);
        void prepare_for_new_game();

        player_number_t connected_players_count() const;

        std::set<std::string> playernames;
    
    private:
        std::map<sockaddr, con_state_t, sockaddr_cmp> connections;

        player_number_t ready_players = 0;
        player_number_t connected_players = 0;
        player_number_t connected_clients = 0;

        void add_connection(const cts_t& req);
        void remove_connection(map_iter con);
        bool try_toggle_ready_state(map_iter& con, const cts_t& req);
        int verify_connection(const con_state_t& con, const cts_t& req) const;
        bool verify_unique_playername(const std::string& playername) const;

        player_number_t get_player_index(const std::string& playername) const;
};

#endif /* CONNECTION_MANAGER_H */
