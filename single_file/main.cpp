#include <algorithm>
#include <chrono>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

enum class Direction {
    Up,
    Down,
    Left,
    Right,
    Invalid
};

enum class CommandType {
    Move,
    Restart,
    Quit,
    Invalid
};

struct PlayerCommand {
    CommandType type;
    Direction direction;
};

class InputHandler {
public:
    char getInput() const {
        std::cout << "\n请输入操作：";
        std::string line;
        if (!std::getline(std::cin, line)) {
            return 'q';
        }

        if (line.empty()) {
            return '\n';
        }

        return line[0];
    }

    bool isMoveInput(char ch) const {
        Direction direction = toDirection(ch);
        return direction != Direction::Invalid;
    }

    Direction toDirection(char ch) const {
        switch (normalize(ch)) {
        case 'w':
            return Direction::Up;
        case 's':
            return Direction::Down;
        case 'a':
            return Direction::Left;
        case 'd':
            return Direction::Right;
        default:
            return Direction::Invalid;
        }
    }

    PlayerCommand parseInput(char ch) const {
        char normalized = normalize(ch);

        if (isMoveInput(normalized)) {
            return {CommandType::Move, toDirection(normalized)};
        }

        if (normalized == 'r') {
            return {CommandType::Restart, Direction::Invalid};
        }

        if (normalized == 'q') {
            return {CommandType::Quit, Direction::Invalid};
        }

        return {CommandType::Invalid, Direction::Invalid};
    }

private:
    char normalize(char ch) const {
        return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
};

class RandomGenerator {
public:
    RandomGenerator()
        : engine(static_cast<unsigned int>(
              std::chrono::steady_clock::now().time_since_epoch().count())) {
    }

    int generateTileValue() {
        std::uniform_int_distribution<int> distribution(1, 10);
        return distribution(engine) == 10 ? 4 : 2;
    }

    int generateIndex(int max) {
        if (max <= 0) {
            return 0;
        }

        std::uniform_int_distribution<int> distribution(0, max - 1);
        return distribution(engine);
    }

private:
    std::mt19937 engine;
};

class Board {
public:
    static const int SIZE = 4;

    Board()
        : grid(SIZE, std::vector<int>(SIZE, 0)) {
        reset();
    }

    void reset() {
        grid.assign(SIZE, std::vector<int>(SIZE, 0));
        addRandomTile();
        addRandomTile();
    }

    bool move(Direction direction, int& gainedScore) {
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

    bool moveLeft(int& gainedScore) {
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

    bool moveRight(int& gainedScore) {
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

    bool moveUp(int& gainedScore) {
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

    bool moveDown(int& gainedScore) {
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

    bool canMove() const {
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

    bool has2048() const {
        for (const auto& row : grid) {
            for (int value : row) {
                if (value == 2048) {
                    return true;
                }
            }
        }

        return false;
    }

    bool hasEmptyCell() const {
        for (const auto& row : grid) {
            for (int value : row) {
                if (value == 0) {
                    return true;
                }
            }
        }

        return false;
    }

    std::vector<std::pair<int, int>> getEmptyCells() const {
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

    void addRandomTile() {
        std::vector<std::pair<int, int>> emptyCells = getEmptyCells();

        if (emptyCells.empty()) {
            return;
        }

        int index = randomGenerator.generateIndex(static_cast<int>(emptyCells.size()));
        int row = emptyCells[index].first;
        int col = emptyCells[index].second;
        grid[row][col] = randomGenerator.generateTileValue();
    }

    const std::vector<std::vector<int>>& getGrid() const {
        return grid;
    }

private:
    std::vector<std::vector<int>> grid;
    RandomGenerator randomGenerator;

    std::vector<int> mergeLine(const std::vector<int>& line, int& gainedScore) const {
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
};

class ScoreManager {
public:
    ScoreManager()
        : score(0), bestScore(0) {
    }

    void resetScore() {
        score = 0;
    }

    void addScore(int value) {
        if (value <= 0) {
            return;
        }

        score += value;
        updateBestScore();
    }

    int getScore() const {
        return score;
    }

    int getBestScore() const {
        return bestScore;
    }

    void updateBestScore() {
        if (score > bestScore) {
            bestScore = score;
        }
    }

private:
    int score;
    int bestScore;
};

class Renderer {
public:
    void clearScreen() const {
        std::cout << std::string(40, '\n');
    }

    void render(const Board& board, int score, int bestScore) const {
        clearScreen();

        std::cout << "================ 2048 控制台版 ================\n";
        std::cout << "当前分数: " << score << "    最高分: " << bestScore << "\n\n";

        printSeparator();
        for (const auto& row : board.getGrid()) {
            std::cout << "|";
            for (int value : row) {
                if (value == 0) {
                    std::cout << std::setw(6) << "."
                              << "|";
                } else {
                    std::cout << std::setw(6) << value
                              << "|";
                }
            }
            std::cout << "\n";
            printSeparator();
        }

        std::cout << "\n操作: W/w 上移  A/a 左移  S/s 下移  D/d 右移\n";
        std::cout << "      R/r 重新开始  Q/q 退出游戏\n";
    }

    void showWelcome() const {
        std::cout << "欢迎来到 2048 控制台版游戏。\n";
    }

    void showGameOver() const {
        std::cout << "\n游戏结束：棋盘已满且没有可以合并的相邻数字。\n";
        std::cout << "输入 R 重新开始，或输入 Q 退出。\n";
    }

    void showWin() const {
        std::cout << "\n恭喜，你已经合成 2048！可以继续挑战更高分。\n";
    }

    void showInvalidInput() const {
        std::cout << "\n输入无效，请输入 W/A/S/D/R/Q。\n";
    }

    void showInvalidMove() const {
        std::cout << "\n本次移动无效：棋盘没有发生变化，不会生成新数字。\n";
    }

    void showExit() const {
        std::cout << "\n已退出游戏，感谢游玩。\n";
    }

    void showMessage(const std::string& message) const {
        if (!message.empty()) {
            std::cout << "\n" << message << "\n";
        }
    }

private:
    void printSeparator() const {
        std::cout << "+------+------+------+------+\n";
    }
};

class Game {
public:
    Game()
        : board(),
          renderer(),
          inputHandler(),
          scoreManager(),
          isRunning(true),
          hasWon(false),
          isGameOver(false),
          statusMessage("游戏开始：已随机生成两个数字。") {
    }

    void run() {
        renderer.showWelcome();

        while (isRunning) {
            renderer.render(board, scoreManager.getScore(), scoreManager.getBestScore());
            renderer.showMessage(statusMessage);

            if (isGameOver) {
                renderer.showGameOver();
            }

            char input = inputHandler.getInput();
            processInput(input);
        }

        renderer.showExit();
    }

    void restart() {
        board.reset();
        scoreManager.resetScore();
        hasWon = false;
        isGameOver = false;
        statusMessage = "已重新开始游戏。";
    }

    void processInput(char input) {
        PlayerCommand command = inputHandler.parseInput(input);

        switch (command.type) {
        case CommandType::Move: {
            if (isGameOver) {
                statusMessage = "游戏已经结束，请先重新开始或退出。";
                return;
            }

            int gainedScore = 0;
            bool moved = board.move(command.direction, gainedScore);

            if (!moved) {
                statusMessage = "本次移动无效：棋盘没有发生变化。";
                return;
            }

            scoreManager.addScore(gainedScore);
            statusMessage = gainedScore > 0
                ? "有效移动，合并得分 +" + std::to_string(gainedScore) + "。"
                : "有效移动，已生成一个新数字。";

            checkGameState();
            break;
        }
        case CommandType::Restart:
            restart();
            break;
        case CommandType::Quit:
            isRunning = false;
            break;
        case CommandType::Invalid:
        default:
            statusMessage = "输入无效，请输入 W/A/S/D/R/Q。";
            break;
        }
    }

    void checkGameState() {
        if (board.has2048() && !hasWon) {
            hasWon = true;
            statusMessage += " 恭喜，你已经合成 2048！";
        }

        if (!board.canMove()) {
            isGameOver = true;
            statusMessage += " 棋盘已满且无法继续移动，游戏失败。";
        }
    }

private:
    Board board;
    Renderer renderer;
    InputHandler inputHandler;
    ScoreManager scoreManager;
    bool isRunning;
    bool hasWon;
    bool isGameOver;
    std::string statusMessage;
};

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Game game;
    game.run();
    return 0;
}
