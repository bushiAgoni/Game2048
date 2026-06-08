#include "Renderer.h"

#include <iomanip>
#include <iostream>
#include <string>

void Renderer::clearScreen() const {
    std::cout << std::string(40, '\n');
}

void Renderer::render(const Board& board, int score, int bestScore) const {
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

void Renderer::showWelcome() const {
    std::cout << "欢迎来到 2048 控制台版游戏。\n";
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
    std::cout << "+------+------+------+------+\n";
}
