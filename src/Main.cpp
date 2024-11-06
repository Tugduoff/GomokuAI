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
#include "Commands.hpp"

Gomoku::Board board;

int main(void)
{
    std::string cmd;

    while (1) {
        std::cin >> cmd;

        if (cmd == "START") {
            uint8_t size = 20;
            std::cin >> size; // Not used yet

            std::cout << "OK" << std::endl;
        } else if (cmd == "TURN") {
            uint8_t x = 0;
            uint8_t y = 0;

            std::cin >> x >> y;

            // board.board[x][y] = 2; // enemy move

            // ia.turn(board);
        } else if (cmd == "BEGIN") {
            // ia.turn(board);
        } else if (cmd == "BOARD") {
            std::string line;
            uint8_t x = 0;
            uint8_t y = 0;
            uint8_t color = 0;

            board = Gomoku::Board(); // Reset the board
            while (1) {
                std::cin >> line;
                if (line == "DONE")
                    break;

                std::cin >> x >> y >> color;

                board.board[x][y] = color; // enemy or player move (1 or 2)

            }
            // ia.turn(board);
        } else if (cmd == "INFO") {
            std::string key;
            uint64_t value;

            if (key == "folder") {
                std::string folder;
                std::cin >> key >> folder;
            } else if (key == "evaluate") {
                uint8_t x = 0;
                uint8_t y = 0;

                std::cin >> key >> x >> y;
            } else {
                std::cin >> key >> value;
            }
            return 0;
        } else if (cmd == "ABOUT") {
            std::cout << "name=\"Centurion\", version=\"1.0\", author=\"Tugdu & Traz\", country=\"France\"" << std::endl;
        } else if (cmd == "END") {
            return 0;
        }
    }
    return 0;
}
