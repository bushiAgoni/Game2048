#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "ScoreManager.h"

#include <string>

class Game {
public:
    Game();

    void run();
    void restart();
    void processInput(char input);
    void checkGameState();

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

#endif
