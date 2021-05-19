#ifndef TYPES_H
#define TYPES_H

#include <ctime>
#include <stdint.h>
#include <unistd.h>

using port_t           = uint16_t;
using seed_t           = uint32_t;
using turn_speed_t     = int16_t;
using rounds_per_sec_t = uint16_t;
using dimensions_t     = uint32_t;

constexpr port_t           DEFAULT_PORT           = 2021;
const     seed_t           DEFAULT_SEED           = time(NULL);
constexpr turn_speed_t     DEFAULT_TURNING_SPEED  = 6; // Sprawdzić, jakie są rozsądne limity na tę wartość. Na pewno nie chcemy równej 0, ale ujemne ok.
constexpr rounds_per_sec_t DEFAULT_ROUNDS_PER_SEC = 50;
constexpr dimensions_t     DEFAULT_BOARD_WIDTH    = 640;
constexpr dimensions_t     DEFAULT_BOARD_HEIGHT   = 480;

constexpr int64_t MIN_PORT           = 1024;
constexpr int64_t MIN_SEED           = 0;
constexpr int64_t MIN_TURNING_SPEED  = -1000;
constexpr int64_t MIN_ROUNDS_PER_SEC = 1;
constexpr int64_t MIN_BOARD_WIDTH    = 1;
constexpr int64_t MIN_BOARD_HEIGHT   = 1;

constexpr int64_t MAX_PORT           = 49151;
constexpr int64_t MAX_SEED           = UINT32_MAX;
constexpr int64_t MAX_TURNING_SPEED  = 1000;
constexpr int64_t MAX_ROUNDS_PER_SEC = 1000;
constexpr int64_t MAX_BOARD_WIDTH    = 4000;
constexpr int64_t MAX_BOARD_HEIGHT   = 4000;

#endif /* TYPES_H */