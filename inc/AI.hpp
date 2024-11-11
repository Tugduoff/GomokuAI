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
                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << std::endl;

                std::cout << (int)x << "," << (int)y << std::endl;
            }

            // Attributes
            Board board;

        protected:
        private:
    };
};

#endif // AI_HPP
