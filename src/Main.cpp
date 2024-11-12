/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Main
*/

#include <sstream>
#include "CommandHandler.hpp"

int main(void) {
    Gomoku::AI ai;
    Gomoku::CommandHandler handler(ai);
    std::string cmd;

    while (true) {
        std::cin.clear();
        cmd.clear();
        if (!(std::cin >> cmd)) {
            std::cout << "DEBUG End of input detected. Exiting" << std::endl;
            break;
        }
        handler.execute(cmd);
    }
    return 0;
}
