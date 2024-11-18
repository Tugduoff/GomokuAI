/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Board Class
*/

#ifndef BOARD_HPP_
    #define BOARD_HPP_

    #include <vector>
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
            ~Board() = default;

            Board(const Board &other) {
                for (uint8_t i = 0; i < 20; ++i) {
                    for (uint8_t j = 0; j < 20; ++j) {
                        board[i][j] = other.board[i][j];
                    }
                }
                lines = other.lines;
            }

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

            Color board[20][20] = { Color::EMPTY };
            std::vector<std::array<Line, 4>> lines;

            Board &operator=(const Board &other);

        protected:
        private:
    };
};

#endif // BOARD_HPP_
