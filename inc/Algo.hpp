/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Algo
*/

#ifndef ALGO_HPP_
    #define ALGO_HPP_

    #include <chrono>
    #include <limits>
    #include <iostream>
    #include <array>
    #include <TranspositionTable.hpp>
    #include <any>
    #include "AI.hpp"

namespace Gomoku {
    /**
     * @brief This contains the algorithm of the game
     */
    class Algo {
        public:
            Algo();
            Algo(Gomoku::AI &ai) : __ai(ai) {};
            ~Algo();

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
            Position computeFirstEvaluation(Board &board, Board &searchBoard);

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
                std::chrono::time_point<std::chrono::high_resolution_clock> end, int &maxDepth);

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
                bool isMaximizing, int alpha, int beta);

            /**
             * @brief Generate the moves to explore for the principalVariationSearch function
             *
             * @param boardTmp
             * @return std::vector<Position>
             */
            std::vector<Position> generateMoves(Board& boardTmp);

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
                int alpha, int beta);

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
                int alpha, int beta);

        protected:
        private:
            AI __ai;
    };
};

#endif // BOARD_HPP_
