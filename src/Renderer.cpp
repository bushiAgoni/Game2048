#include "Renderer.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

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

void Renderer::clearScreen() const {
    std::cout << "\033[2J\033[H";
}

void Renderer::render(
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

void Renderer::showWelcome() const {
    std::cout << "正在启动 2048 控制台游戏...\n";
}

void Renderer::showGameOver() const {
    std::cout << "\n游戏结束：棋盘已满且没有可以合并的相邻数字。\n";
    std::cout << "输入 R 重新开始，或输入 Q 退出。\n";
}

void Renderer::showWin() const {
    std::cout << "\n恭喜，你已经合成 2048！可以继续挑战更高分。\n";
}

void Renderer::showInvalidInput() const {
    std::cout << "\n输入无效，请输入 W/A/S/D/R/Q。\n";
}

void Renderer::showInvalidMove() const {
    std::cout << "\n本次移动无效：棋盘没有发生变化，不会生成新数字。\n";
}

void Renderer::showExit() const {
    std::cout << "\n已退出游戏，感谢游玩。\n";
}

void Renderer::showMessage(const std::string& message) const {
    if (!message.empty()) {
        std::cout << "\n" << message << "\n";
    }
}

void Renderer::printSeparator() const {
    std::cout << FG_MUTED << "+--------+--------+--------+--------+" << RESET << "\n";
}

void Renderer::printHeader() const {
    std::cout << BOLD << FG_TEXT;
    std::cout << "+--------------------------------------------------+\n";
    std::cout << "|  2048 控制台交互版                               |\n";
    std::cout << "+--------------------------------------------------+\n";
    std::cout << RESET;
    std::cout << FG_MUTED << "  C++17 面向对象实现  |  彩色终端 UI  |  单键操作\n\n" << RESET;
}

void Renderer::printScoreCards(int score, int bestScore, int moveCount, int maxTile) const {
    std::cout << PANEL << FG_TEXT << "  分数 " << RESET << " "
              << BOLD << FG_YELLOW << std::setw(6) << score << RESET << "   ";
    std::cout << PANEL << FG_TEXT << "  最高分 " << RESET << " "
              << BOLD << FG_GREEN << std::setw(6) << bestScore << RESET << "   ";
    std::cout << PANEL << FG_TEXT << "  步数 " << RESET << " "
              << BOLD << FG_CYAN << std::setw(4) << moveCount << RESET << "   ";
    std::cout << PANEL << FG_TEXT << "  最大数字 " << RESET << " "
              << BOLD << FG_CYAN << std::setw(5) << maxTile << RESET << "\n\n";
}

void Renderer::printBoard(const Board& board) const {
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

void Renderer::printProgress(int maxTile) const {
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

void Renderer::printStatus(const std::string& statusMessage, bool isGameOver, bool hasWon) const {
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

void Renderer::printControls() const {
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

std::string Renderer::tileStyle(int value) const {
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

std::string Renderer::centerText(const std::string& text, int width) const {
    if (static_cast<int>(text.size()) >= width) {
        return text.substr(0, width);
    }

    int leftPadding = (width - static_cast<int>(text.size())) / 2;
    int rightPadding = width - static_cast<int>(text.size()) - leftPadding;
    return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
}
