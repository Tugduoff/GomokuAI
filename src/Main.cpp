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
            std::cin >> size;

            std::cout << "OK" << std::endl;
        } else if (cmd == "TURN") {
            uint8_t x = 0;
            uint8_t y = 0;

            std::cin >> x >> y;
            Gomoku::Position pos(x, y);
            // Gomoku::Stone stone(pos, ia.getColor());
            // board.board[x][y] = stone;

            // ia.turn(board);
        } else if (cmd == "BEGIN") {
            // ia.setColor(Gomoku::Color::BLACK);

            // ia.turn(board);
        } else if (cmd == "BOARD") {
            board = Gomoku::Board();
            while (1) {
                std::string line;
                std::cin >> line;
                if (line == "DONE") {
                    break;
                }
                uint8_t x = 0;
                uint8_t y = 0;
                uint8_t color = 0;

                std::cin >> x >> y >> color;
                Gomoku::Position pos(x, y);
                // Gomoku::Stone stone(pos, ia.getColor(color));
                // board.board[x][y] = stone;

                // ia.turn(board);
            }
        } else if (cmd == "INFO") {
            std::string key;
            int value;

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
