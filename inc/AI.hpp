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
             * @brief Turn of the AI \n
             * Coordinates are given by the bot for the next move
             */
            void turnCommand()
            {
                uint8_t x {0};
                uint8_t y {0};

                // For testing purposes, we will just return the first empty cell //
                for (uint8_t i = 0; i < 20; i++) {
                    for (uint8_t j = 0; j < 20; j++) {
                        if (_board.board[i][j].color == Color::NONE) {
                            x = i;
                            y = j;
                            break;
                        }
                    }
                }
                ////
                std::cout << x << "," << y << std::endl;
            }

            // Attributes
            Board _board;

        protected:
        private:
    };
};

#endif // AI_HPP
