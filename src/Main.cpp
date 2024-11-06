/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Main
*/

#include <string>
#include <iostream>
#include "Board.hpp"
#include "Commands.hpp"

Gomoku::Board board;

int main(void)
{
    Commands commands;
    std::string line;

    while (1) {
        std::cin >> line;

        if (line == "START") {
            commands.startCommand();
            uint8_t size = 20;
            std::cin >> size;
        }
    }
    return 0;
}
