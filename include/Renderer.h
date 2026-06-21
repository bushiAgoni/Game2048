#ifndef RENDERER_H
#define RENDERER_H

#include "Board.h"

#include <string>

class Renderer {
public:
    void clearScreen() const;
    void render(
        const Board& board,
        int score,
        int bestScore,
        int moveCount,
        const std::string& statusMessage,
        bool isGameOver,
        bool hasWon
    ) const;
    void showWelcome() const;
    void showGameOver() const;
    void showWin() const;
    void showInvalidInput() const;
    void showInvalidMove() const;
    void showExit() const;
    void showMessage(const std::string& message) const;

private:
    void printSeparator() const;
    void printHeader() const;
    void printScoreCards(int score, int bestScore, int moveCount, int maxTile) const;
    void printBoard(const Board& board) const;
    void printProgress(int maxTile) const;
    void printStatus(const std::string& statusMessage, bool isGameOver, bool hasWon) const;
    void printControls() const;
    std::string tileStyle(int value) const;
    std::string centerText(const std::string& text, int width) const;
};

#endif
