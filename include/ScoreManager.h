#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

class ScoreManager {
public:
    ScoreManager();

    void resetScore();
    void addScore(int value);
    int getScore() const;
    int getBestScore() const;
    void updateBestScore();

private:
    int score;
    int bestScore;
};

#endif
