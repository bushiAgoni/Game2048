#include "Game.h"

#include <string>

Game::Game()
    : board(),
      renderer(),
      inputHandler(),
      scoreManager(),
      isRunning(true),
      hasWon(false),
      isGameOver(false),
      statusMessage("游戏开始：已随机生成两个数字。") {
}

void Game::run() {
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

void Game::restart() {
    board.reset();
    scoreManager.resetScore();
    hasWon = false;
    isGameOver = false;
    statusMessage = "已重新开始游戏。";
}

void Game::processInput(char input) {
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

void Game::checkGameState() {
    if (board.has2048() && !hasWon) {
        hasWon = true;
        statusMessage += " 恭喜，你已经合成 2048！";
    }

    if (!board.canMove()) {
        isGameOver = true;
        statusMessage += " 棋盘已满且无法继续移动，游戏失败。";
    }
}
