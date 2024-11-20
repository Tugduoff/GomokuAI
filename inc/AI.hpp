/*
** EPITECH PROJECT: Gomoku, 2024
** File description:
** AI class
*/

#ifndef AI_HPP
    #define AI_HPP

    #include <chrono>
    #include <limits>
    #include <iostream>
    #include <array>
    #include <TranspositionTable.hpp>
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
            AI() {
                tt.initializeZobristTable();
            };

            /**
             * @brief Destroy the AI object
             *
             */
            ~AI() = default;

            /**
             * @brief Turn of the AI
             * Coordinates are given by the bot for the next move
             */
            void turn();

            void addToSearchBoard(uint8_t x, uint8_t y, uint8_t color);

            void removeFromSearchBoard(uint8_t x, uint8_t y);

            void displaySearchBoard();

            /**
             * @brief Evaluate the board
             *
             * @return int
             *
             * This function evaluates the board and returns a score
             * If the score is positive, the AI is in a winning position
             * If the score is negative, the AI is in a losing position
             *
             * If the score is 1000000, the AI has won
             * If the score is -1000000, the AI has lost
             */
            int evaluateBoard(bool debug = false);

            // Attributes
            TranspositionTable tt;
            Board board;
            Board searchBoard; // Search board contains each cell to be evaluated
            std::vector<std::array<Stone, 9>> searchBoardMoves;
            int maxDepth = 5;
            std::array<int, 10> maxCellsForDepth = { 400, 100, 25, 15, 8, 5, 0, 0, 0, 0 };
    };
};

#endif // AI_HPP
