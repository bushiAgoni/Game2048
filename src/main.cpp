#include "Game.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (outputHandle != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(outputHandle, &mode)) {
            SetConsoleMode(outputHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif

    Game game;
    game.run();
    return 0;
}
