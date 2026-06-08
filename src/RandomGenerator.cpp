#include "RandomGenerator.h"

#include <chrono>

RandomGenerator::RandomGenerator()
    : engine(static_cast<unsigned int>(
          std::chrono::steady_clock::now().time_since_epoch().count())) {
}

int RandomGenerator::generateTileValue() {
    std::uniform_int_distribution<int> distribution(1, 10);
    return distribution(engine) == 10 ? 4 : 2;
}

int RandomGenerator::generateIndex(int max) {
    if (max <= 0) {
        return 0;
    }

    std::uniform_int_distribution<int> distribution(0, max - 1);
    return distribution(engine);
}
