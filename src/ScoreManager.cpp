#include "ScoreManager.h"

ScoreManager::ScoreManager()
    : score(0), bestScore(0) {
}

void ScoreManager::resetScore() {
    score = 0;
}

void ScoreManager::addScore(int value) {
    if (value <= 0) {
        return;
    }

    score += value;
    updateBestScore();
}

int ScoreManager::getScore() const {
    return score;
}

int ScoreManager::getBestScore() const {
    return bestScore;
}

void ScoreManager::updateBestScore() {
    if (score > bestScore) {
        bestScore = score;
    }
}
