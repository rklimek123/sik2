#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <unistd.h>

#include <chrono>

using port_t           = uint16_t;
using seed_t           = uint32_t;
using turn_speed_t     = int16_t;
using rounds_per_sec_t = uint16_t;
using dimensions_t     = uint32_t;

constexpr port_t           DEFAULT_PORT           = 2021;
const     seed_t           DEFAULT_SEED           = time(NULL);
constexpr turn_speed_t     DEFAULT_TURNING_SPEED  = 6;
constexpr rounds_per_sec_t DEFAULT_ROUNDS_PER_SEC = 50;
constexpr dimensions_t     DEFAULT_BOARD_WIDTH    = 640;
constexpr dimensions_t     DEFAULT_BOARD_HEIGHT   = 480;

constexpr int64_t MIN_PORT           = 1024;
constexpr int64_t MIN_SEED           = 0;
constexpr int64_t MIN_TURNING_SPEED  = 0;
constexpr int64_t MIN_ROUNDS_PER_SEC = 1;
constexpr int64_t MIN_BOARD_WIDTH    = 1;
constexpr int64_t MIN_BOARD_HEIGHT   = 1;

constexpr int64_t MAX_PORT           = 49151;
constexpr int64_t MAX_SEED           = UINT32_MAX;
constexpr int64_t MAX_TURNING_SPEED  = 359;
constexpr int64_t MAX_ROUNDS_PER_SEC = 200;
constexpr int64_t MAX_BOARD_WIDTH    = 4000;
constexpr int64_t MAX_BOARD_HEIGHT   = 4000;

using player_number_t  = uint8_t;
constexpr player_number_t MAX_NUMBER_OF_PLAYERS = 25;

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

inline uint64_t delay_ns(const TimePoint& newer, const TimePoint& older) {
    uint64_t nanoseconds = std::chrono::duration<uint64_t, std::nano>(newer-older).count();
    return nanoseconds;
}

inline uint64_t delay_ms(const TimePoint& newer, const TimePoint& older) {
    return delay_ns(newer, older) / 1e6;
}

#endif /* TYPES_H */