#include "InputHandler.h"

#include <cctype>
#include <iostream>
#include <string>

char InputHandler::getInput() const {
    std::cout << "\n请输入操作：";
    std::string line;
    if (!std::getline(std::cin, line)) {
        return 'q';
    }

    if (line.empty()) {
        return '\n';
    }

    return line[0];
}

bool InputHandler::isMoveInput(char ch) const {
    Direction direction = toDirection(ch);
    return direction != Direction::Invalid;
}

Direction InputHandler::toDirection(char ch) const {
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

PlayerCommand InputHandler::parseInput(char ch) const {
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

char InputHandler::normalize(char ch) const {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}
