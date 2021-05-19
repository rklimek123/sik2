#include "game-state.h"

void GameState::parse_port(const char* num_str) {
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

void GameState::parse_seed(const char* num_str) {
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

void GameState::parse_turning_speed(const char* num_str) {
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

void GameState::parse_rounds_per_sec(const char* num_str) {
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

void GameState::parse_board_width(const char* num_str) {
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

void GameState::parse_board_height(const char* num_str) {
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

void GameState::parse_input_parameters(int argc, char* argv[]) {
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

