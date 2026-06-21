#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
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
        std::cout << "\n请选择操作：";

#ifdef _WIN32
        if (_isatty(_fileno(stdin))) {
            int ch = _getch();

            if (ch == 0 || ch == 224) {
                int arrow = _getch();
                switch (arrow) {
                case 72:
                    std::cout << "↑\n";
                    return 'w';
                case 80:
                    std::cout << "↓\n";
                    return 's';
                case 75:
                    std::cout << "←\n";
                    return 'a';
                case 77:
                    std::cout << "→\n";
                    return 'd';
                default:
                    std::cout << "\n";
                    return '\n';
                }
            }

            std::cout << static_cast<char>(ch) << "\n";
            return static_cast<char>(ch);
        }
#endif

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

    int getMaxTile() const {
        int maxTile = 0;

        for (const auto& row : grid) {
            for (int value : row) {
                if (value > maxTile) {
                    maxTile = value;
                }
            }
        }

        return maxTile;
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

namespace {
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";
const std::string FG_TEXT = "\033[38;5;255m";
const std::string FG_MUTED = "\033[38;5;248m";
const std::string FG_CYAN = "\033[38;5;51m";
const std::string FG_YELLOW = "\033[38;5;220m";
const std::string FG_GREEN = "\033[38;5;120m";
const std::string FG_RED = "\033[38;5;203m";
const std::string PANEL = "\033[48;5;236m";
const std::string PANEL_DARK = "\033[48;5;234m";
const int TILE_WIDTH = 8;
}

class Renderer {
public:
    void clearScreen() const {
        std::cout << "\033[2J\033[H";
    }

    void render(
        const Board& board,
        int score,
        int bestScore,
        int moveCount,
        const std::string& statusMessage,
        bool isGameOver,
        bool hasWon
    ) const {
        clearScreen();
        printHeader();
        printScoreCards(score, bestScore, moveCount, board.getMaxTile());
        printProgress(board.getMaxTile());
        printBoard(board);
        printStatus(statusMessage, isGameOver, hasWon);
        printControls();
    }

    void showWelcome() const {
        std::cout << "正在启动 2048 控制台游戏...\n";
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
        std::cout << FG_MUTED << "+--------+--------+--------+--------+" << RESET << "\n";
    }

    void printHeader() const {
        std::cout << BOLD << FG_TEXT;
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "|  2048 控制台交互版                               |\n";
        std::cout << "+--------------------------------------------------+\n";
        std::cout << RESET;
        std::cout << FG_MUTED << "  C++17 面向对象实现  |  彩色终端 UI  |  单键操作\n\n" << RESET;
    }

    void printScoreCards(int score, int bestScore, int moveCount, int maxTile) const {
        std::cout << PANEL << FG_TEXT << "  分数 " << RESET << " "
                  << BOLD << FG_YELLOW << std::setw(6) << score << RESET << "   ";
        std::cout << PANEL << FG_TEXT << "  最高分 " << RESET << " "
                  << BOLD << FG_GREEN << std::setw(6) << bestScore << RESET << "   ";
        std::cout << PANEL << FG_TEXT << "  步数 " << RESET << " "
                  << BOLD << FG_CYAN << std::setw(4) << moveCount << RESET << "   ";
        std::cout << PANEL << FG_TEXT << "  最大数字 " << RESET << " "
                  << BOLD << FG_CYAN << std::setw(5) << maxTile << RESET << "\n\n";
    }

    void printBoard(const Board& board) const {
        printSeparator();

        for (const auto& row : board.getGrid()) {
            std::cout << FG_MUTED << "|" << RESET;
            for (int value : row) {
                if (value == 0) {
                    std::cout << PANEL_DARK << DIM << centerText(".", TILE_WIDTH) << RESET;
                } else {
                    std::cout << tileStyle(value) << BOLD
                              << centerText(std::to_string(value), TILE_WIDTH) << RESET;
                }
                std::cout << FG_MUTED << "|" << RESET;
            }
            std::cout << "\n";
            printSeparator();
        }
    }

    void printProgress(int maxTile) const {
        const int target = 2048;
        int value = std::max(2, maxTile);
        int level = 1;

        while (value > 2) {
            value /= 2;
            ++level;
        }

        int filled = std::min(22, level * 22 / 11);
        std::cout << FG_MUTED << "  目标进度 2 -> " << target << "  " << RESET;
        std::cout << FG_CYAN << "[";
        for (int index = 0; index < 22; ++index) {
            std::cout << (index < filled ? "#" : "-");
        }
        std::cout << "]" << RESET << "  ";
        std::cout << BOLD << FG_YELLOW << maxTile << RESET << "\n\n";
    }

    void printStatus(const std::string& statusMessage, bool isGameOver, bool hasWon) const {
        std::string color = FG_CYAN;
        std::string label = "状态";

        if (isGameOver) {
            color = FG_RED;
            label = "失败";
        } else if (hasWon) {
            color = FG_GREEN;
            label = "胜利";
        }

        std::cout << color << "[ " << label << " ] " << RESET
                  << FG_TEXT << statusMessage << RESET << "\n\n";
    }

    void printControls() const {
        std::cout << FG_MUTED << "  操作面板" << RESET << "\n";
        std::cout << "        " << PANEL << FG_TEXT << "   W / ↑   " << RESET << "\n";
        std::cout << "  " << PANEL << FG_TEXT << "   A / ←   " << RESET
                  << "  " << PANEL << FG_TEXT << "   S / ↓   " << RESET
                  << "  " << PANEL << FG_TEXT << "   D / →   " << RESET << "\n";
        std::cout << "  " << FG_MUTED << "R 重新开始    Q 退出游戏";
#ifdef _WIN32
        std::cout << "    Windows 下支持方向键和单键输入";
#endif
        std::cout << RESET << "\n";
    }

    std::string tileStyle(int value) const {
        switch (value) {
        case 2:
            return "\033[48;5;230m\033[38;5;232m";
        case 4:
            return "\033[48;5;222m\033[38;5;232m";
        case 8:
            return "\033[48;5;215m\033[38;5;232m";
        case 16:
            return "\033[48;5;203m\033[38;5;255m";
        case 32:
            return "\033[48;5;168m\033[38;5;255m";
        case 64:
            return "\033[48;5;99m\033[38;5;255m";
        case 128:
            return "\033[48;5;45m\033[38;5;232m";
        case 256:
            return "\033[48;5;120m\033[38;5;232m";
        case 512:
            return "\033[48;5;220m\033[38;5;232m";
        case 1024:
            return "\033[48;5;208m\033[38;5;232m";
        case 2048:
            return "\033[48;5;198m\033[38;5;255m";
        default:
            return "\033[48;5;255m\033[38;5;232m";
        }
    }

    std::string centerText(const std::string& text, int width) const {
        if (static_cast<int>(text.size()) >= width) {
            return text.substr(0, width);
        }

        int leftPadding = (width - static_cast<int>(text.size())) / 2;
        int rightPadding = width - static_cast<int>(text.size()) - leftPadding;
        return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
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
          moveCount(0),
          statusMessage("游戏开始：已随机生成两个数字。") {
    }

    void run() {
        renderer.showWelcome();

        while (isRunning) {
            renderer.render(
                board,
                scoreManager.getScore(),
                scoreManager.getBestScore(),
                moveCount,
                statusMessage,
                isGameOver,
                hasWon
            );

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
        moveCount = 0;
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
            ++moveCount;
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
    int moveCount;
    std::string statusMessage;
};

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (outputHandle != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(outputHandle, &mode)) {
            SetConsoleMode(outputHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif

    Game game;
    game.run();
    return 0;
}
