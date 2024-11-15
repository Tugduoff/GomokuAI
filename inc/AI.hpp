/*
** EPITECH PROJECT: Gomoku, 2024
** File description:
** AI class
*/

#ifndef AI_HPP
    #define AI_HPP

    #include <chrono>
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

                status = evaluateBoard(true);
                addToSearchBoard(x, y, 1);

                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << std::endl;
                std::cout << "DEBUG Status: " << status << std::endl;
                std::cout << "DEBUG Max depth: " << maxDepth << std::endl;
                if (status == std::numeric_limits<int>::max()) {
                    std::cout << "DEBUG WIN" << std::endl;
                } else if (status == std::numeric_limits<int>::min()) {
                    std::cout << "DEBUG LOSE" << std::endl;
                }
                std::cout << (int)x << "," << (int)y << std::endl; // Send the move to the game manager
            }

            /**
             * @brief Add the 8 surrounding cells of a position given to the search board + the position itself
             *
             * the 8 surrounding cells are added as : 1
             * the cell itself is added as : 2
             *
             * @param x : x position
             * @param y : y position
             */
            void addToSearchBoard(uint8_t x, uint8_t y, uint8_t color) {
                std::cout << "DEBUG Adding pos : " << (int)x << "," << (int)y << " to search board" << std::endl;
                searchBoard.board[x][y] = (color == 1) ? Color::AI_NEW : Color::ENEMY_NEW;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if (x + i >= 0 && x + i < 20 && y + j >= 0 && y + j < 20 && searchBoard.board[x + i][y + j] == Color::EMPTY)
                            searchBoard.board[x + i][y + j] = Color::TO_EXPLORE;
                    }
                }
                displaySearchBoard();
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
                        if (searchBoard.board[i][j] == Color::AI_NEW)
                            searchBoard.board[i][j] = Color::AI;
                        else if (searchBoard.board[i][j] == Color::ENEMY_NEW)
                            searchBoard.board[i][j] = Color::ENEMY;
                    }
                    std::cout << std::endl;
                }
            }

            // Attributes
            Board board;
            Board searchBoard; // Search board contains each cell to be evaluated
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

            std::unordered_map<size_t, int> transpositionTable;

            size_t hashBoard(const Board& boardTmp) {
                std::hash<std::string> hasher;
                std::string boardString;
                for (const auto& row : boardTmp.board) {
                    for (const auto& cell : row) {
                        boardString += std::to_string((uint8_t)cell);
                    }
                }
                return hasher(boardString);
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
                // size_t boardHash = hashBoard(exploratingBoard);
                //if (transpositionTable.find(boardHash) != transpositionTable.end()) {
                //    return transpositionTable[boardHash];
                //}
                int score = evaluateBoard();

                if (depth == 0 || score >= 1000000 || score <= -1000000) {
                    return score;
                }

                std::vector<Position> moves = generateMoves(exploratingBoard);

                if (isMaximizing) {
                    bool firstChild = true;
                    for (const auto& move : moves) {
                        exploratingBoard.board[move.x][move.y] = Color::ENEMY;
                        //for (uint)
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
                        exploratingBoard.board[move.x][move.y] = Color::TO_EXPLORE;
                        board.undoMove(move);
                        alpha = std::max(alpha, res);
                        if (alpha >= beta) {
                            //transpositionTable[boardHash] = alpha;
                            return alpha;
                        }
                    }
                    // transpositionTable[boardHash] = alpha;
                    return alpha;
                } else {
                    bool firstChild = true;
                    for (const auto& move : moves) {
                        exploratingBoard.board[move.x][move.y] = Color::AI;
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
                        exploratingBoard.board[move.x][move.y] = Color::TO_EXPLORE;
                        board.undoMove(move);
                        beta = std::min(beta, res);
                        if (alpha >= beta) {
                            //transpositionTable[boardHash] = beta;
                            return beta;
                        }
                    }
                    // transpositionTable[boardHash] = beta;
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

                if (seconds >= 1 && maxDepth > 0)
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
