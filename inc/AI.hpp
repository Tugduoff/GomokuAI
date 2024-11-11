/*
** EPITECH PROJECT: Gomoku, 2024
** File description:
** AI class
*/

#ifndef AI_HPP
    #define AI_HPP

    #include <iostream>
    #include "Stone.hpp"
    #include "Board.hpp"

namespace Gomoku {
    /**
     * @brief Class for the AI
     *
     * This class contains the AI of the game
     */
    class AI {
        public:
            /**
             * @brief Construct a new AI object
             *
             */
            AI() = default;

            /**
             * @brief Destroy the AI object
             *
             */
            ~AI() = default;

            /**
             * @brief Turn of the AI
             * Coordinates are given by the bot for the next move
             */
            void turn()
            {
                uint8_t x = 0;
                uint8_t y = 0;

                while (board.board[x][y] != 0) {
                    x++;
                    if (x == 20) {
                        x = 0;
                        y++;
                    }
                    if (y == 20) {
                        std::cout << "ERROR can't find any position in my board" << std::endl;
                        return;
                    }
                }
                board.board[x][y] = 1;
                int status = evaluateBoard();
                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << " new board status: " << status << std::endl;

                std::cout << (int)x << "," << (int)y << std::endl;
            }

            // Attributes
            Board board;

        protected:
        private:

            /**
             * @brief Evaluate the board
             * 
             * @return int
             * 
             * This function evaluates the board and returns a score
             * If the score is positive, the AI is in a winning position
             * If the score is negative, the AI is in a losing position
             * 
             * If the score is 10000, the AI has won
             * If the score is -10000, the AI has lost
             */
            int evaluateBoard() {
                const int WIN_SCORE = 10000;
                const int LOSS_SCORE = -10000;

                for (uint8_t x = 0; x < 20; ++x) {
                    for (uint8_t y = 0; y < 20; ++y) {
                        if (board.board[x][y] != 0) {
                            uint8_t color = board.board[x][y];

                            if (checkWin(color, x, y)) {
                                return color == 1 ? WIN_SCORE : LOSS_SCORE;
                            };
                        }
                    }
                }
                return 0;
            }

            /**
             * @brief Check if the player of the color given has won
             * 
             * @param color
             * @param x
             * @param y
             * 
             * @return true : the player has won
             * @return false : the player has not won
             * 
             * This function checks if the player has won
             */
            int checkWin(uint8_t color, uint8_t x, uint8_t y) {
                if ((y <= 15 && checkFiveInRow(x, y, 0, 1, color)) ||
                    (x <= 15 && checkFiveInRow(x, y, 1, 0, color)) ||
                    (x <= 15 && y <= 15 && checkFiveInRow(x, y, 1, 1, color)) ||
                    (x >= 4 && y <= 15 && checkFiveInRow(x, y, -1, 1, color)))
                    return true;
                return false;
            }

            /**
             * @brief Check if there are 5 stones in a row
             * 
             * @param x : x position of the starting stone
             * @param y : y position of the starting stone
             * @param dx : x direction
             * @param dy : y direction
             * @param player : player color
             * 
             * @return true : there are 5 stones in a row
             * @return false : there are not 5 stones in a row
             * 
             * This function checks if there are 5 stones in a row starting from the given position
             */
            bool checkFiveInRow(int x, int y, int dx, int dy, int player) {
                for (int i = 0; i < 5; ++i) {
                    if (board.board[x + i * dx][y + i * dy] != player)
                        return false;
                }
                return true;
            }

    };
};

#endif // AI_HPP
