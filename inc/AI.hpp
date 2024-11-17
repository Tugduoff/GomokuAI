/*
** EPITECH PROJECT: Gomoku, 2024
** File description:
** AI class
*/

#ifndef AI_HPP
    #define AI_HPP

    #include <chrono>
    #include <limits>
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
            void turn()
            {
                // displaySearchBoard();
                int status = evaluateBoard();
                std::cout << "DEBUG Status: " << status << std::endl;
                if (status == std::numeric_limits<int>::max()) {
                    std::cout << "DEBUG WIN" << std::endl;
                } else if (status == std::numeric_limits<int>::min()) {
                    std::cout << "DEBUG LOSE" << std::endl;
                }

                Position bestMove = getBestMove();
                uint8_t x = bestMove.x;
                uint8_t y = bestMove.y;

                board.playMove(bestMove, Color::AI);

                status = evaluateBoard();
                addToSearchBoard(x, y, 1);
                // displaySearchBoard();

                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << std::endl;
                std::cout << "DEBUG Status: " << status << std::endl;
                std::cout << "DEBUG Max depth: " << maxDepth << std::endl;
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
            int maxDepth = 4;

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
             * If the score is 1000000, the AI has won
             * If the score is -1000000, the AI has lost
             */
            int evaluateBoard(bool debug = false) {
                int score = 0;

                for (auto &lines : board.lines) {
                    for (auto &line : lines) {
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

            std::vector<Position> generateMoves(Board& boardTmp) {
                std::vector<Position> moves;
                for (uint8_t x = 0; x < 20; ++x) {
                    for (uint8_t y = 0; y < 20; ++y) {
                        if (boardTmp.board[x][y] == Color::TO_EXPLORE) {
                            moves.push_back(Position(x, y));
                        }
                    }
                }
                return moves;
            }

            void stockIntoTranspositionTable(uint64_t zobristKey, uint8_t depth, int bestValue, int alpha, int beta) {
                TranspositionTable entry;

                entry.value = bestValue;
                entry.depth = depth;
                if (bestValue <= alpha) {
                    entry.flag = -1;
                } else if (bestValue >= beta) {
                    entry.flag = 1;
                } else {
                    entry.flag = 0;
                }
                tt.transpositionTable[zobristKey] = entry;
            }

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
            int principalVariationSearch(Board exploratingBoard, int depth, bool isMaximizing, int alpha, int beta) {
                uint64_t zobristKey = tt.computeZobristHash(exploratingBoard);
                auto it = tt.transpositionTable.find(zobristKey);

                if (it != tt.transpositionTable.end()) {
                    const TranspositionTable& entry = it->second;
                    if (entry.depth >= depth) {
                        if (entry.flag == 0) {
                            return entry.value;
                        } else if (entry.flag == -1 && entry.value <= alpha) {
                            return entry.value;
                        } else if (entry.flag == 1 && entry.value >= beta) {
                            return entry.value;
                        }
                    }
                }

                int score = evaluateBoard();
                if (depth == 0 || score >= 1000000 || score <= -1000000) {
                    return score;
                }

                std::vector<Position> moves = generateMoves(exploratingBoard);

                if (isMaximizing) {
                    bool firstChild = true;
                    for (const auto& move : moves) {
                        addToSearchBoard(move.x, move.y, 2);
                        board.playMove(move, Color::ENEMY);
                        int res;
                        if (firstChild) {
                            res = principalVariationSearch(exploratingBoard, depth - 1, false, alpha, beta);
                            firstChild = false;
                        } else {
                            res = principalVariationSearch(exploratingBoard, depth - 1, false, alpha, alpha + 1);
                            if (res > alpha && res < beta) {
                                res = principalVariationSearch(exploratingBoard, depth - 1, false, alpha, beta);
                            }
                        }
                        removeFromSearchBoard(move.x, move.y);
                        board.undoMove(move);
                        alpha = std::max(alpha, res);
                    }
                    stockIntoTranspositionTable(zobristKey, depth, alpha, alpha, beta);
                    return alpha;
                } else {
                    bool firstChild = true;
                    for (const auto& move : moves) {
                        addToSearchBoard(move.x, move.y, 1);
                        board.playMove(move, Color::AI);
                        int res;
                        if (firstChild) {
                            res = principalVariationSearch(exploratingBoard, depth - 1, true, alpha, beta);
                            firstChild = false;
                        } else {
                            res = principalVariationSearch(exploratingBoard, depth - 1, true, beta - 1, beta);
                            if (res > alpha && res < beta) {
                                res = principalVariationSearch(exploratingBoard, depth - 1, true, alpha, beta);
                            }
                        }
                        removeFromSearchBoard(move.x, move.y);
                        board.undoMove(move);
                        beta = std::min(beta, res);
                    }
                    stockIntoTranspositionTable(zobristKey, depth, beta, alpha, beta);
                    return beta;
                }
            }

            /**
             * @brief Get the best move for the AI
             *
             * @return Position : the best move for the AI
             */
            Position getBestMove() {
                int bestScore = std::numeric_limits<int>::min();
                Position bestMove;
                int depth = maxDepth;

                auto getBestMoveStart = std::chrono::high_resolution_clock::now();
                for (uint8_t x = 0; x < 20; ++x) {
                    for (uint8_t y = 0; y < 20; ++y) {
                        if (searchBoard.board[x][y] == Color::TO_EXPLORE) {
                            searchBoard.board[x][y] = Color::AI;
                            board.playMove(Position(x, y), Color::AI);

                            int score = principalVariationSearch(searchBoard, depth, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

                            searchBoard.board[x][y] = Color::TO_EXPLORE;
                            board.undoMove(Position(x, y));
                            if (score == bestScore) {
                                if (rand() % 2 == 0) {
                                    bestScore = score;
                                    bestMove = Position(x, y);
                                }
                            } else if (score > bestScore) {
                                bestScore = score;
                                bestMove = Position(x, y);
                            }

                        }
                    }
                }
                auto getBestMoveEnd = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(getBestMoveEnd - getBestMoveStart).count();
                int seconds = duration / 1'000'000;
                int milliseconds = (duration % 1'000'000) / 1'000;
                int microseconds = duration % 1'000;

                std::cout << "DEBUG Execution time for getBestMove : " << seconds << "s "
                << milliseconds << "ms "
                << microseconds << "µs" << std::endl;

                if ((seconds > 0 && maxDepth > 0) || (milliseconds > 500 && maxDepth > 0))
                    maxDepth--;

                std::cout << "DEBUG Best move found: " << (int)bestMove.x << "," << (int)bestMove.y << " with score: " << bestScore << " using depth: " << maxDepth + 1 << std::endl;
                if (bestScore == std::numeric_limits<int>::min()) {
                    bestMove = Position(10, 10);
                }
                return bestMove;
            }
    };
};

#endif // AI_HPP
