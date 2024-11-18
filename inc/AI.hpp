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
            void turn(Position bestMove)
            {
                displaySearchBoard();
                int status = evaluateBoard();
                std::cout << "DEBUG Status: " << status << std::endl;
                if (status == std::numeric_limits<int>::max()) {
                    std::cout << "DEBUG WIN" << std::endl;
                } else if (status == std::numeric_limits<int>::min()) {
                    std::cout << "DEBUG LOSE" << std::endl;
                }

                int cellsToExplore = searchBoard.count(Color::TO_EXPLORE);

                while (cellsToExplore > maxCellsForDepth[maxDepth]) {
                    std::cout << "DEBUG Reducing depth" << std::endl;
                    maxDepth--;
                }
                std::cout << "DEBUG Cells to explore: " << cellsToExplore << std::endl;
                std::cout << "DEBUG Max depth: " << maxDepth + 1 << std::endl;

                uint8_t x = bestMove.x;
                uint8_t y = bestMove.y;

                board.playMove(bestMove, Color::AI);

                status = evaluateBoard();
                addToSearchBoard(x, y, 1);
                // displaySearchBoard();

                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << std::endl;
                std::cout << "DEBUG Status: " << status << std::endl;
                std::cout << "DEBUG Max depth: " << maxDepth + 1 << std::endl;
                if (status >= 10000000) {
                    std::cout << "DEBUG WIN" << std::endl;
                    displaySearchBoard();
                } else if (status <= -10000000) {
                    std::cout << "DEBUG LOSE" << std::endl;
                    displaySearchBoard();
                }
                std::cout << (int)x << "," << (int)y << std::endl; // Send the move to the game manager
            }

            void addToSearchBoard(uint8_t x, uint8_t y, uint8_t color) {
                // Store old colors for each 9 cells
                std::array<Stone, 9> oldPositions;

                // Store the center position explicitly
                oldPositions[4] = Stone(Position(x, y), searchBoard.board[x][y]);
                searchBoard.board[x][y] = (Color)color;

                // Iterate through the 3x3 grid centered at (x, y)
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if (i == 0 && j == 0)
                            continue;
                        int idx = (i + 1) * 3 + (j + 1); // 3x3 grid index calculation
                        Position pos(x + i, y + j);

                        if (searchBoard.isPosEmpty(pos)) {
                            oldPositions[idx] = Stone(pos, searchBoard.board[pos.x][pos.y]);
                            searchBoard.board[pos.x][pos.y] = Color::TO_EXPLORE;
                        } else {
                            // For out-of-bounds positions, store a dummy value
                            oldPositions[idx] = Stone(Position(-1, -1), Color::OUT_OF_BOUND);
                        }
                    }
                }

                // Push the stored state onto the stack
                searchBoardMoves.push_back(oldPositions);
                // std::cout << "DEBUG | Added in search board: " << searchBoardMoves.size() << " stone: " << (int)oldPositions[4].pos.x << "," << (int)oldPositions[4].pos.y << std::endl;
            }

            void removeFromSearchBoard(uint8_t x, uint8_t y) {
                // Retrieve the last stored state
                std::array<Stone, 9> oldPositions = searchBoardMoves.back();
                searchBoardMoves.pop_back(); // Remove the last element from the stack
                // std::cout << "DEBUG | Removed from search board: " << searchBoardMoves.size() << std::endl;

                // Restore the center position
                searchBoard.board[x][y] = oldPositions[4].color;

                // Iterate through the 3x3 grid centered at (x, y)
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int idx = (i + 1) * 3 + (j + 1); // 3x3 grid index calculation
                        const Stone &stone = oldPositions[idx];

                        if (stone.color != Color::OUT_OF_BOUND) {
                            // std::cout << "DEBUG | Restoring: " << (int)stone.pos.x << "," << (int)stone.pos.y << " color: " << stone.color << std::endl;
                            searchBoard.board[stone.pos.x][stone.pos.y] = stone.color;
                        }
                    }
                }
            }

            void displaySearchBoard() {
                // DISPLAY SEARCH BOARD
                for (int i = 0; i < 20; ++i) {
                    std::cout << "DEBUG | ";
                    if (i == 0) {
                        // Display the y axis
                        std::cout << "    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19" << std::endl;
                        std::cout << "DEBUG | ";
                    }
                    for (int j = 0; j < 20; ++j) {
                        if (j == 0) {
                            // Display the x axis
                            if (i < 10)
                                std::cout << " " << i << "  ";
                            else
                                std::cout << i << "  ";
                        }
                        std::cout << searchBoard.board[i][j] << "  ";
                    }
                    std::cout << std::endl;
                }
            }

            // Attributes
            TranspositionTable tt;
            Board board;
            Board searchBoard; // Search board contains each cell to be evaluated
            std::vector<std::array<Stone, 9>> searchBoardMoves;
            int maxDepth = 10;
            std::array<int, 10> maxCellsForDepth = { 400, 150, 50, 25, 15, 8, 5, 3, 2, 1 };

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
            int evaluateBoard(bool debug = false) {
                int score = 0;

                for (auto &lines : board.lines) {
                    for (auto &line : lines) {
                        if (line.score == std::numeric_limits<int>::max()) {
                            return std::numeric_limits<int>::max();
                        } else if (line.score == std::numeric_limits<int>::min()) {
                            return std::numeric_limits<int>::min();
                        }
                        score += line.score;
                    }
                }

                if (debug) {
                    for (auto &lines : board.lines) {
                        for (auto &line : lines) {
                            std::cout << "DEBUG " << line;
                        }
                    }
                }
                return score;
            }
    };
};

#endif // AI_HPP
