#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

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
    char getInput() const;
    bool isMoveInput(char ch) const;
    Direction toDirection(char ch) const;
    PlayerCommand parseInput(char ch) const;

private:
    char normalize(char ch) const;
};

#endif
