#ifndef BOARD_H
#define BOARD_H

#include "InputHandler.h"
#include "RandomGenerator.h"

#include <utility>
#include <vector>

class Board {
public:
    static const int SIZE = 4;

    Board();

    void reset();
    bool move(Direction direction, int& gainedScore);

    bool moveLeft(int& gainedScore);
    bool moveRight(int& gainedScore);
    bool moveUp(int& gainedScore);
    bool moveDown(int& gainedScore);

    bool canMove() const;
    bool has2048() const;
    bool hasEmptyCell() const;
    int getMaxTile() const;
    std::vector<std::pair<int, int>> getEmptyCells() const;
    void addRandomTile();

    const std::vector<std::vector<int>>& getGrid() const;

private:
    std::vector<std::vector<int>> grid;
    RandomGenerator randomGenerator;

    std::vector<int> mergeLine(const std::vector<int>& line, int& gainedScore) const;
};

#endif
