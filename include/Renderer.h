#ifndef RENDERER_H
#define RENDERER_H

#include "Board.h"

#include <string>

class Renderer {
public:
    void clearScreen() const;
    void render(const Board& board, int score, int bestScore) const;
    void showWelcome() const;
    void showGameOver() const;
    void showWin() const;
    void showInvalidInput() const;
    void showInvalidMove() const;
    void showExit() const;
    void showMessage(const std::string& message) const;

private:
    void printSeparator() const;
};

#endif
