#include "Board.h"

#include <algorithm>

Board::Board()
    : grid(SIZE, std::vector<int>(SIZE, 0)) {
    reset();
}

void Board::reset() {
    grid.assign(SIZE, std::vector<int>(SIZE, 0));
    addRandomTile();
    addRandomTile();
}

bool Board::move(Direction direction, int& gainedScore) {
    gainedScore = 0;
    bool changed = false;

    switch (direction) {
    case Direction::Up:
        changed = moveUp(gainedScore);
        break;
    case Direction::Down:
        changed = moveDown(gainedScore);
        break;
    case Direction::Left:
        changed = moveLeft(gainedScore);
        break;
    case Direction::Right:
        changed = moveRight(gainedScore);
        break;
    default:
        return false;
    }

    if (changed) {
        addRandomTile();
    }

    return changed;
}

bool Board::moveLeft(int& gainedScore) {
    bool changed = false;

    for (int row = 0; row < SIZE; ++row) {
        int rowScore = 0;
        std::vector<int> merged = mergeLine(grid[row], rowScore);

        if (merged != grid[row]) {
            changed = true;
        }

        grid[row] = merged;
        gainedScore += rowScore;
    }

    return changed;
}

bool Board::moveRight(int& gainedScore) {
    bool changed = false;

    for (int row = 0; row < SIZE; ++row) {
        std::vector<int> reversed = grid[row];
        std::reverse(reversed.begin(), reversed.end());

        int rowScore = 0;
        std::vector<int> merged = mergeLine(reversed, rowScore);
        std::reverse(merged.begin(), merged.end());

        if (merged != grid[row]) {
            changed = true;
        }

        grid[row] = merged;
        gainedScore += rowScore;
    }

    return changed;
}

bool Board::moveUp(int& gainedScore) {
    bool changed = false;

    for (int col = 0; col < SIZE; ++col) {
        std::vector<int> column;
        for (int row = 0; row < SIZE; ++row) {
            column.push_back(grid[row][col]);
        }

        int columnScore = 0;
        std::vector<int> merged = mergeLine(column, columnScore);

        for (int row = 0; row < SIZE; ++row) {
            if (grid[row][col] != merged[row]) {
                changed = true;
            }
            grid[row][col] = merged[row];
        }

        gainedScore += columnScore;
    }

    return changed;
}

bool Board::moveDown(int& gainedScore) {
    bool changed = false;

    for (int col = 0; col < SIZE; ++col) {
        std::vector<int> column;
        for (int row = 0; row < SIZE; ++row) {
            column.push_back(grid[row][col]);
        }

        std::reverse(column.begin(), column.end());
        int columnScore = 0;
        std::vector<int> merged = mergeLine(column, columnScore);
        std::reverse(merged.begin(), merged.end());

        for (int row = 0; row < SIZE; ++row) {
            if (grid[row][col] != merged[row]) {
                changed = true;
            }
            grid[row][col] = merged[row];
        }

        gainedScore += columnScore;
    }

    return changed;
}

bool Board::canMove() const {
    if (hasEmptyCell()) {
        return true;
    }

    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (col + 1 < SIZE && grid[row][col] == grid[row][col + 1]) {
                return true;
            }
            if (row + 1 < SIZE && grid[row][col] == grid[row + 1][col]) {
                return true;
            }
        }
    }

    return false;
}

bool Board::has2048() const {
    for (const auto& row : grid) {
        for (int value : row) {
            if (value == 2048) {
                return true;
            }
        }
    }

    return false;
}

bool Board::hasEmptyCell() const {
    for (const auto& row : grid) {
        for (int value : row) {
            if (value == 0) {
                return true;
            }
        }
    }

    return false;
}

std::vector<std::pair<int, int>> Board::getEmptyCells() const {
    std::vector<std::pair<int, int>> emptyCells;

    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (grid[row][col] == 0) {
                emptyCells.push_back({row, col});
            }
        }
    }

    return emptyCells;
}

void Board::addRandomTile() {
    std::vector<std::pair<int, int>> emptyCells = getEmptyCells();

    if (emptyCells.empty()) {
        return;
    }

    int index = randomGenerator.generateIndex(static_cast<int>(emptyCells.size()));
    int row = emptyCells[index].first;
    int col = emptyCells[index].second;
    grid[row][col] = randomGenerator.generateTileValue();
}

const std::vector<std::vector<int>>& Board::getGrid() const {
    return grid;
}

std::vector<int> Board::mergeLine(const std::vector<int>& line, int& gainedScore) const {
    std::vector<int> numbers;
    for (int value : line) {
        if (value != 0) {
            numbers.push_back(value);
        }
    }

    std::vector<int> merged;
    for (std::size_t index = 0; index < numbers.size(); ++index) {
        // 相邻且相等的两个数字只在本轮合并一次，合并后跳过下一个数字。
        if (index + 1 < numbers.size() && numbers[index] == numbers[index + 1]) {
            int combined = numbers[index] * 2;
            merged.push_back(combined);
            gainedScore += combined;
            ++index;
        } else {
            merged.push_back(numbers[index]);
        }
    }

    while (merged.size() < SIZE) {
        merged.push_back(0);
    }

    return merged;
}
