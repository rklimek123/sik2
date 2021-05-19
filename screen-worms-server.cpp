#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip6.h>

#include <iostream>
#include <string>

#include "err.h"
#include "game-state.h"
#include "types.h"

namespace {
    port_t           port           = DEFAULT_PORT;
    seed_t           seed           = DEFAULT_SEED;
    turn_speed_t     turning_speed  = DEFAULT_TURNING_SPEED;
    rounds_per_sec_t rounds_per_sec = DEFAULT_ROUNDS_PER_SEC;
    dimensions_t     board_width    = DEFAULT_BOARD_WIDTH;
    dimensions_t     board_height   = DEFAULT_BOARD_HEIGHT;

    extern "C" void syserr(const char *fmt, ...);

    int set_up_listen_sock() {
        int sock;
        struct sockaddr_in6 addr;

        sock = socket(AF_INET6, SOCK_DGRAM, 0);
        if (sock < 0) {
            syserr("socket_v6");
        }
        
        addr.sin6_family = AF_INET6;
        addr.sin6_addr = in6addr_any;
        addr.sin6_port = htons(port);
        
        if (bind(sock, (struct sockaddr *) &addr, (socklen_t) sizeof(addr)) < 0) {
            syserr("bind_v6");
        }
        
        return sock;
    }


}

int main(int argc, char* argv[]) {
    int listen_sock;
    int multicast_sock;

    GameState game(argc, argv);
    listen_sock = set_up_listen_sock();


    
    std::cout << "cool";
}