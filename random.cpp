#include "random.h"

Random::random_t Random::rand() {
    random_t result = next_random;

    uint64_t temp = (uint64_t)result;
    next_random = (random_t)((temp * 279410273) % 4294967291);
    
    return result;
}
