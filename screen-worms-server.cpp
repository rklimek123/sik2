#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip6.h>

#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <set>
#include <string>
#include <utility>

#include "client-to-server.h"
#include "server-to-client.h"
#include "connection-manager.h"
#include "err.h"
#include "game-state.h"
#include "player.h"
#include "types.h"

namespace {
    typedef struct client_info {

    } client_info_t;

    constexpr const char* program_arguments = "p:s:t:v:w:h:";

    port_t           port           = DEFAULT_PORT;
    seed_t           seed           = DEFAULT_SEED;
    turn_speed_t     turning_speed  = DEFAULT_TURNING_SPEED;
    rounds_per_sec_t rounds_per_sec = DEFAULT_ROUNDS_PER_SEC;
    dimensions_t     board_width    = DEFAULT_BOARD_WIDTH;
    dimensions_t     board_height   = DEFAULT_BOARD_HEIGHT;

    int listen_socket;

    extern "C" void syserr(const char *fmt, ...);

    ConnectionManager connection_manager;


    void parse_port(const char* num_str) {
        char* res;
        int64_t unrefined_num = strtol(num_str, &res, 10);
        
        if (*res) {
            std::cerr << "The supplied port must be a number" << std::endl;
            exit(1);
        }

        if (unrefined_num >= MIN_PORT && unrefined_num <= MAX_PORT) {
            port = (port_t)unrefined_num;
        }
        else {
            std::cerr << "The supplied port must fit between "
                        << MIN_PORT << " and " << MAX_PORT << std::endl;
            exit(1);
        }
    }

    void parse_seed(const char* num_str) {
        char* res;
        int64_t unrefined_num = strtol(num_str, &res, 10);
        
        if (*res) {
            std::cerr << "The supplied seed must be a number" << std::endl;
            exit(1);
        }

        if (unrefined_num >= MIN_SEED && unrefined_num <= MAX_SEED) {
            seed = (seed_t)unrefined_num;
        }
        else {
            std::cerr << "The supplied seed must fit between "
                        << MIN_SEED << " and " << MAX_SEED << std::endl;
            exit(1);
        }
    }

    void parse_turning_speed(const char* num_str) {
        char* res;
        int64_t unrefined_num = strtol(num_str, &res, 10);
        
        if (*res) {
            std::cerr << "The supplied turning speed must be a number" << std::endl;
            exit(1);
        }

        if (unrefined_num >= MIN_TURNING_SPEED && unrefined_num <= MAX_TURNING_SPEED) {
            turning_speed = (turn_speed_t)unrefined_num;
        }
        else {
            std::cerr << "The supplied turning speed must fit between "
                        << MIN_TURNING_SPEED << " and " << MAX_TURNING_SPEED << std::endl;
            exit(1);
        }
    }

    void parse_rounds_per_sec(const char* num_str) {
        char* res;
        int64_t unrefined_num = strtol(num_str, &res, 10);
        
        if (*res) {
            std::cerr << "The supplied rounds per second must be a number" << std::endl;
            exit(1);
        }

        if (unrefined_num >= MIN_ROUNDS_PER_SEC && unrefined_num <= MAX_ROUNDS_PER_SEC) {
            rounds_per_sec = (rounds_per_sec_t)unrefined_num;
        }
        else {
            std::cerr << "The supplied rounds per second must fit between "
                        << MIN_ROUNDS_PER_SEC << " and " << MAX_ROUNDS_PER_SEC << std::endl;
            exit(1);
        }
    }

    void parse_board_width(const char* num_str) {
        char* res;
        int64_t unrefined_num = strtol(num_str, &res, 10);
        
        if (*res) {
            std::cerr << "The supplied board width must be a number" << std::endl;
            exit(1);
        }

        if (unrefined_num >= MIN_BOARD_WIDTH && unrefined_num <= MAX_BOARD_WIDTH) {
            board_width = (dimensions_t)unrefined_num;
        }
        else {
            std::cerr << "The supplied board width must fit between "
                        << MIN_BOARD_WIDTH << " and " << MAX_BOARD_WIDTH << std::endl;
            exit(1);
        }
    }

    void parse_board_height(const char* num_str) {
        char* res;
        int64_t unrefined_num = strtol(num_str, &res, 10);
        
        if (*res) {
            std::cerr << "The supplied board height must be a number" << std::endl;
            exit(1);
        }

        if (unrefined_num >= MIN_BOARD_HEIGHT && unrefined_num <= MAX_BOARD_HEIGHT) {
            board_height = (dimensions_t)unrefined_num;
        }
        else {
            std::cerr << "The supplied board height must fit between "
                        << MIN_BOARD_HEIGHT << " and " << MAX_BOARD_HEIGHT << std::endl;
            exit(1);
        }
    }

    void parse_input_parameters(int argc, char* argv[]) {
        int opt;

        while ((opt = getopt(argc, argv, program_arguments)) != -1) {
            switch (opt) {
                case 'p':
                    parse_port(optarg);
                    break;

                case 's':
                    parse_seed(optarg);
                    break;
                
                case 't':
                    parse_turning_speed(optarg);
                    break;
                
                case 'v':
                    parse_rounds_per_sec(optarg);
                    break;
                
                case 'w':
                    parse_board_width(optarg);
                    break;
                
                case 'h':
                    parse_board_height(optarg);
                    break;

                default:
                    std::cerr << "Usage: " << argv[0] << " [-p n] [-s n] [-t n] [-v n] [-w n] [-h n], "
                                << "where n is a parameter for each of the options" << std::endl;
                    exit(1);
            }
        }
    }


    void set_up_listen_socket() {
    /*
        struct sockaddr_in6 addr;

        listen_socket = socket(AF_INET6, SOCK_DGRAM, 0);
        if (listen_socket < 0) {
            syserr("socket");
        }
        
        addr.sin6_family = AF_INET6;
        addr.sin6_addr = in6addr_any;
        addr.sin6_port = htons(port);
    */
        struct sockaddr_in addr;

        listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (listen_socket < 0) {
            syserr("socket");
        }
        
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);

        if (bind(listen_socket, (struct sockaddr *) &addr, (socklen_t) sizeof(addr)) < 0) {
            syserr("bind");
        }
    }

    void set_nonblock(bool turn) {
        int flags = fcntl(listen_socket, F_GETFL, 0);
        flags = turn ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
        if (fcntl(listen_socket, F_SETFL, flags) != 0)
            syserr("fcntl switch nonblocking");
    }




    void gather_players() {
        bool need_start;

        while (!need_start) {
            cts_t req;
            int ret = read_from_client(listen_socket, req, false);
            if (ret > 0) {
                set_nonblock(true);
                need_start = connection_manager.handle_request_nogame(req);
                send_to_client_blank(listen_socket, &req.client_address, req.client_addr_len);
            }

            if (!need_start)
                usleep(1000 * CLIENT_REQUEST_DELAY_MS);
        }
    }
}

int main(int argc, char* argv[]) {
    int listen_sock;

    parse_input_parameters(argc, argv);
    set_up_listen_socket();
    
    bool l = false;
    for (;;) {
        gather_players();
    }

    // GameState game(seed, turning_speed, board_width, board_height);
}
