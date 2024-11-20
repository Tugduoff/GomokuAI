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

            void displaySearchBoard(Position &pos);

            int crossPatternBonus(Line &first, Line &second);

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

            /**
             * @brief Return the best move for the AI
             *
             * @return Position
             */
            Position getBestMove();

            /**
             * @brief Compute the first evaluation of the board
             *
             * @param board : the board to evaluate
             * @param searchBoard : the search board
             * @return Position
             */
            Position computeFirstEvaluation();

            /**
             * @brief Check the score of the move
             *
             * @param bestScore : the best score
             * @param bestMove : the best move
             * @param score : the score
             * @param move : the move
             * @return bool
             */
            bool checkScore(int &bestScore, Position &bestMove, int &score, Position &move);

            /**
             * @brief Display the execution time of the function getBestMove
             */
            void displayExecutionTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
                std::chrono::time_point<std::chrono::high_resolution_clock> end);

            /**
             * @brief Stock the values into the transposition table
             *
             * @param zobristKey
             * @param depth
             * @param bestValue
             * @param alpha
             * @param beta
             */
            void stockIntoTranspositionTable(uint64_t &zobristKey, uint8_t &depth,
                int &bestValue, int &alpha, int &beta);

            /**
             * @brief Minimax algorithm- elegage Alpha-Beta pruning. \n
             *
             * This function is a recursive function that will explore the board and return the score of the board.
             * The first part of the function is for the maximizing player (AI) \n
             * and the second part is for the minimizing player (Enemy)
             *
             * @param exploratingBoard : the board to explore where 1 is the cell to explore
             * @param depth : the depth of the search
             * @param isMaximizing : if the AI is maximizing or minimizing
             * @param alpha : the alpha value
             * @param beta : the beta value
             * @return int : the score of the board
             */
            int principalVariationSearch(Board &exploratingBoard, uint8_t depth,
                bool isMaximizing, int alpha, int beta, std::chrono::time_point<std::chrono::high_resolution_clock> &start);

            /**
             * @brief Generate the moves to explore for the principalVariationSearch function
             *
             * @param boardTmp
             * @return std::vector<Position>
             */
            std::vector<Position> generateMoves(Board& boardTmp);

            bool isTimeToStop(std::chrono::time_point<std::chrono::high_resolution_clock> &start);

            /**
             * @brief Do the max part of the principalVariationSearch function
             *
             * @param exploratingBoard
             * @param zobristKey
             * @param depth
             * @param alpha
             * @param beta
             * @return int
             */
            int doMax(Board &exploratingBoard, uint64_t &zobristKey, uint8_t depth,
                int alpha, int beta, std::chrono::time_point<std::chrono::high_resolution_clock> &start);

            /**
             * @brief Do the min part of the principalVariationSearch function
             *
             * @param exploratingBoard
             * @param zobristKey
             * @param depth
             * @param alpha
             * @param beta
             * @return int
             */
            int doMin(Board &exploratingBoard, uint64_t &zobristKey, uint8_t depth,
                int alpha, int beta, std::chrono::time_point<std::chrono::high_resolution_clock> &start);

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
