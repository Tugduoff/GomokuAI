/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Board Class
*/

#ifndef BOARD_HPP_
    #define BOARD_HPP_

    #include <vector>
    #include <array>
    #include "Line.hpp"

namespace Gomoku {
    /**
     * @brief Class for the board
     * 
     * This class contains the board of the game
     */
    class Board {
        public:
            Board() = default;
            Board(int maxDepth, int timeStop, int D4_pl, int S4_pl, int D3_pl, int S3_pl, int D2_pl,
                int S2_pl, int D4_ai, int S4_ai, int D3_ai, int S3_ai, int D2_ai, int S2_ai) {
                    initLines();
                    individual.D4_pattern_pl = D4_pl;
                    individual.S4_pattern_pl = S4_pl;
                    individual.D3_pattern_pl = D3_pl;
                    individual.S3_pattern_pl = S3_pl;
                    individual.D2_pattern_pl = D2_pl;
                    individual.S2_pattern_pl = S2_pl;
                    individual.D4_pattern_ai = D4_ai;
                    individual.S4_pattern_ai = S4_ai;
                    individual.D3_pattern_ai = D3_ai;
                    individual.S3_pattern_ai = S3_ai;
                    individual.D2_pattern_ai = D2_ai;
                    individual.S2_pattern_ai = S2_ai;
                    individual.timeStop = timeStop;
                    individual.maxDepth = maxDepth;
            };
            ~Board() = default;

            enum Status {
                WIN = 1000000,
                LOSE = -1000000,
                ONGOING = 0
            };

            /**
             * @brief Play a move on the board
             * 
             * This function will play a move on the board
             * 
             * @param pos : the position to play the move
             * @param color : the color of the stone
             */
            void playMove(const Position &pos, Color color);

            /**
             * @brief Undo a move on the board
             * 
             * This function will undo a move on the board
             * 
             * @param pos : the position to undo the move
             */
            void undoMove(const Position &pos);

            /**
             * @brief Check if there are any patterns in the line
             *
             * @param line : the line to check for patterns
             *
             * @note This function will check for patterns in the line and return the power of the line \n
             * @note For the patterns below, we will consider the following board: \n
             * @note X = STONE, + = EMPTY
             */
            int checkPattern(Line &line);

            /**
             * @brief Check if the position is empty
             * 
             * This function will check if the position is empty
             * 
             * @param pos : the position to check
             * @return bool : true if the position is empty, false otherwise
             */
            bool isPosEmpty(const Position &pos);

            /**
             * @brief Get the color at a position
             * 
             * This function will return the color at a position
             * 
             * @param pos : the position to get the color
             * @return Color : the color at the position
             */
            const Color &getAt(const Position &pos);

            /**
             * @brief Check if the position is out of bound
             * 
             * This function will check if the position is out of bound
             * 
             * @param pos : the position to check
             * @return bool : true if the position is out of bound, false otherwise
             */
            bool isOutOfBound(const Position &pos);

            /**
             * @brief Check if there are n stones of the same color in a row
             * 
             * This function will check if there are n stones of the same color in a row
             * 
             * @param pos : the position to start the check
             * @param dx : the x direction
             * @param dy : the y direction
             * @param color : the color to check
             * @param n : the number of stones to check
             * @return bool : true if there are n stones of the same color in a row, false otherwise
             */
            bool checkNColorInRow(const Position &pos, int dx, int dy, const Color &color, int n);

            /**
             * @brief Check if there are n stones of the same color in a row with t triggers
             * 
             * This function will check if there are n stones of the same color in a row with t triggers
             * 
             * @param pos : the position to start the check
             * @param dx : the x direction
             * @param dy : the y direction
             * @param color : the color to check
             * @param n : the number of stones to check
             * @param t : the number of triggers
             * @return bool : true if there are n stones of the same color in a row with t triggers, false otherwise
             */
            bool checkNColorInRowWithTTriggers(const Position &pos, int dx, int dy, const Color &color, int n, int t);

            int count(const Color &color);

            void initLines();

            class Individual {
                public:
                    Individual() = default;
                    ~Individual() = default;

                    int D4_pattern_ai;
                    int S4_pattern_ai;
                    int D3_pattern_ai;
                    int S3_pattern_ai;
                    int D2_pattern_ai;
                    int S2_pattern_ai;

                    int D4_pattern_pl;
                    int S4_pattern_pl;
                    int D3_pattern_pl;
                    int S3_pattern_pl;
                    int D2_pattern_pl;
                    int S2_pattern_pl;

                    int timeStop;
                    int maxDepth;
            };

            Color board[20][20] = { Color::EMPTY };
            std::array<std::array<Line, 4>, 400> boardLines;
            std::vector<Line> uniqueLines;
            Status status = ONGOING;
            std::vector<std::array<Stone, 9>> boardMoves;
            Stone lastMove;
            Individual individual;

        protected:
        private:
    };
};

#endif // BOARD_HPP_
