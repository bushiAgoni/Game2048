#include "InputHandler.h"

#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#endif

char InputHandler::getInput() const {
    std::cout << "\n请选择操作：";

#ifdef _WIN32
    if (_isatty(_fileno(stdin))) {
        int ch = _getch();

        if (ch == 0 || ch == 224) {
            int arrow = _getch();
            switch (arrow) {
            case 72:
                std::cout << "↑\n";
                return 'w';
            case 80:
                std::cout << "↓\n";
                return 's';
            case 75:
                std::cout << "←\n";
                return 'a';
            case 77:
                std::cout << "→\n";
                return 'd';
            default:
                std::cout << "\n";
                return '\n';
            }
        }

        std::cout << static_cast<char>(ch) << "\n";
        return static_cast<char>(ch);
    }
#endif

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
