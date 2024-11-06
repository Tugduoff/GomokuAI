/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Main
*/

#include <string>
#include <iostream>
#include "Board.hpp"
#include "Position.hpp"

Gomoku::Board board;

int main(void)
{
    std::string line;

    while (1) {
        std::cin >> line;

        if (line == "START") {
            uint8_t size = 20;
            std::cin >> size;

            std::cout << "OK" << std::endl;
        } else if (line == "TURN") {
            uint8_t x = 0;
            uint8_t y = 0;

            std::cin >> x >> y;
            Gomoku::Position pos(x, y);
        }
    }
    return 0;
}
