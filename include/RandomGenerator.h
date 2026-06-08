#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>

class RandomGenerator {
public:
    RandomGenerator();

    int generateTileValue();
    int generateIndex(int max);

private:
    std::mt19937 engine;
};

#endif
