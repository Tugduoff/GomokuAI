/*
** EPITECH PROJECT: Gomoku, 2024
** File description:
** AI class
*/

#ifndef AI_HPP
    #define AI_HPP

    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include "Position.hpp"
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
                int status = evaluateBoard();
                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << " new board status: " << status << std::endl;

                std::cout << (int)x << "," << (int)y << std::endl;
            }

            // Attributes
            Board board;

        protected:
        private:
        
            // Threat score, threat stone positions
            std::vector<std::pair<int, std::vector<Position>>> __threats;

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
             * 
             * The score is calculated by checking the threats of the AI and the enemy
             * Each threat score depends on the number of stones in a row it contains
             * 
             * 1 stone in a row: 100 points
             * 2 stones in a row: 300 points
             * 3 stones in a row: 600 points
             * 4 stones in a row: 1000 points
             * 5 stones in a row is a win, so it won't be even computed
             * 
             * For enemy threats, the score is the same but negative
             * 1 stone in a row: -100 points
             * 2 stones in a row: -300 points
             * 3 stones in a row: -600 points
             * 4 stones in a row: -1000 points
             * 
             * The final score is the sum of all the threats in the board
             * 
             * Example: \n
             * 1. 600 : (1,1) (1,2) (1,3) \n
             * 2. 300 : (5,1) (5,2) \n
             * 3. -1000 : (2,2) (2,3) (2,4) (2,5) \n
             * 4. -100 : (5,5)
             * 
             * The final score will be [900 - 1100](-200). The AI is in a losing position
             * 
             * Second example: \n
             * 1. 600 : (1,1) (1,2) (1,3) \n
             * 2. 1000 : (12,2) (12,3) (12,4) (12,5) \n
             * 3. 1000 : (3,3) (3,4) (3,5) (3,6) \n
             * 4. -300 : (5,1) (5,2) \n
             * 5. -600 : (9,1) (9,2) (9,3) \n
             * 6. -600 : (7,1) (7,2) (7,3) \n
             * 7. -600 : (11,1) (11,2) (11,3) \n
             * 
             * The final score will be [2600 - 2100](500). The AI is in a winning position
             */
            int evaluateBoard() {
                const int WIN_SCORE = 1000000;
                const int LOSS_SCORE = -1000000;
                int score = 0;

                for (uint8_t x = 0; x < 20; ++x) {
                    for (uint8_t y = 0; y < 20; ++y) {
                        if (board.board[x][y] != 0) {
                            uint8_t color = board.board[x][y];

                            if (checkWin(color, x, y)) {
                                std::cout << "DEBUG Player: " << (color == 1 ? "AI" : "ENEMY") << " has won" << std::endl;
                                return color == 1 ? WIN_SCORE : LOSS_SCORE;
                            };
                            checkThreat(color, x, y);
                        }
                    }
                }
                removeDuplicateThreats();
                for (auto threat : __threats) {
                    score += threat.first;
                }
                displayThreats(__threats);
                return score;
            }

            /**
             * @brief Check if the player of the color given has won
             * 
             * @param color
             * @param x
             * @param y
             * 
             * @return true : the player has won
             * @return false : the player has not won
             * 
             * This function checks if the player has won
             */
            bool checkWin(uint8_t color, uint8_t x, uint8_t y) {
                if ((y <= 15 && checkFiveInRow(x, y, 0, 1, color)) ||
                    (x <= 15 && checkFiveInRow(x, y, 1, 0, color)) ||
                    (x <= 15 && y <= 15 && checkFiveInRow(x, y, 1, 1, color)) ||
                    (x >= 4 && y <= 15 && checkFiveInRow(x, y, -1, 1, color)))
                    return true;
                return false;
            }

            /**
             * @brief Check if the player of the color given has a threat in the board
             * 
             * @param color
             * @param x
             * @param y
             * 
             * @return true : the player has won
             * @return false : the player has not won
             * 
             * This function checks if the player has won
             */
            void checkThreat(uint8_t color, uint8_t x, uint8_t y) {
                int colorEval = color == 1 ? 1 : -1;
                int threatScore = 0;
                std::vector<Position> threatStones;

                for (int i = 1; i < 6; ++i) {
                    if ((y <= 20 - i && checkNInRow(x, y, 0, 1, color, i))) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x, y + i - 1));
                    } else if (x <= 20 - i && checkNInRow(x, y, 1, 0, color, i)) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x + i - 1, y));
                    } else if (x <= 20 - i && y <= 20 - i && checkNInRow(x, y, 1, 1, color, i)) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x + i - 1, y + i - 1));
                    } else if (x >= i && y <= 20 - i && checkNInRow(x, y, -1, 1, color, i)) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x - i, y + i - 1));
                    } else
                        break;
                }
                if (threatScore != 0)
                    __threats.push_back(std::make_pair(threatScore, threatStones));
            };

            /**
             * @brief Remove duplicate threats
             * 
             * The goal of this function is to remove duplicate threats such as small threats contained by another bigger threat
             * 
             * Example: \n
             * 1. 1000 : (1,1) (1,2) (1,3) \n
             * 2. 300 : (1,1) (1,2) \n
             * 
             * The second threat is contained by the first one, so we remove it
             * 
             * This function will start by sorting the threats by their score (using absolute value)
             * Then, iterate over the threats and check if the positions of the current threat are contained in another threat
             * If they aren't, we add the threat to the unique threats vector
             * 
             * Finally, we set the threats vector to the unique threats vector to keep only the unique threats
             */
            void removeDuplicateThreats() {
                std::vector<std::pair<int, std::vector<Position>>> uniqueThreats;

                std::sort(__threats.begin(), __threats.end(), [](const auto& a, const auto& b) {
                    return std::abs(a.first) > std::abs(b.first);
                });

                for (const auto &currentThreat : __threats) {
                    bool isContained = false;
                    const auto &currentPositions = currentThreat.second;

                    for (const auto &uniqueThreat : uniqueThreats) {
                        const auto &uniquePositions = uniqueThreat.second;

                        if (std::all_of(currentPositions.begin(), currentPositions.end(), [&](const Position &pos) {
                            return std::find(uniquePositions.begin(), uniquePositions.end(), pos) != uniquePositions.end();
                        })) {
                            isContained = true;
                            break;
                        }
                    }

                    if (!isContained)
                        uniqueThreats.push_back(currentThreat);
                }

                __threats = uniqueThreats;
            }

            /**
             * @brief Display the threats
             * 
             * @param threats : the vector of threats
             * 
             * This function displays the threats in the board in this format:
             * 
             * DEBUG Threats:
             * DEBUG Threat score: [score](score of the threat, negative if it's a threat for the enemy) :
             *    [(x,y)](position of one of the threat stones)
             */
            void displayThreats(auto &threats) {
                std::cout << "DEBUG Threats:" << std::endl;
                for (const auto &threat : threats) {
                    std::string positions = "";
                    for (const auto &pos : threat.second) {
                        positions += "  (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")\n";
                    }
                    std::cout << "DEBUG Threat score: " << threat.first << " :\n" << positions;
                }
            }

            /**
             * @brief Check if there are 5 stones in a row
             * 
             * @param x : x position of the starting stone
             * @param y : y position of the starting stone
             * @param dx : x direction
             * @param dy : y direction
             * @param player : player color
             * 
             * @return true : there are 5 stones in a row
             * @return false : there are not 5 stones in a row
             * 
             * This function checks if there are 5 stones in a row starting from the given position
             */
            bool checkFiveInRow(int x, int y, int dx, int dy, int player) {
                for (int i = 0; i < 5; ++i) {
                    if (board.board[x + i * dx][y + i * dy] != player)
                        return false;
                }
                return true;
            }

            /**
             * @brief Check if there are 4 stones in a row
             * 
             * @param x : x position of the starting stone
             * @param y : y position of the starting stone
             * @param dx : x direction
             * @param dy : y direction
             * @param player : player color
             * 
             * @return true : there are 4 stones in a row
             * @return false : there are not 4 stones in a row
             * 
             * This function checks if there are 4 stones in a row starting from the given position
             */
            bool checkFourInRow(int x, int y, int dx, int dy, int player) {
                for (int i = 0; i < 4; ++i) {
                    if (board.board[x + i * dx][y + i * dy] != player)
                        return false;
                }
                return true;
            }

            /**
             * @brief Check if there are n stones in a row
             * 
             * @param x : x position of the starting stone
             * @param y : y position of the starting stone
             * @param dx : x direction
             * @param dy : y direction
             * @param player : player color
             * @param n : number of stones in a row
             * 
             * @return true : there are n stones in a row
             * @return false : there are not n stones in a row
             * 
             * This function checks if there are n stones in a row starting from the given position
             */
            bool checkNInRow(int x, int y, int dx, int dy, int player, int n) {
                for (int i = 0; i < n; ++i) {
                    if (board.board[x + i * dx][y + i * dy] != player)
                        return false;
                }
                return true;
            }

    };
};

#endif // AI_HPP
