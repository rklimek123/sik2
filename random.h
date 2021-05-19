#ifndef RANDOM_H
#define RANDOM_H

#include "types.h"

class Random {
    public:
        using random_t = seed_t;

        Random(): next_random(1) {};
        Random(seed_t seed): next_random((random_t)seed) {};
        random_t rand();
    private:
        random_t next_random;
};

#endif /* RANDOM_H */