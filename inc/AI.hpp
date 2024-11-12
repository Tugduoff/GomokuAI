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
    #include <tuple>
    #include <array>
    #include <limits>
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
                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << std::endl;
                int status = evaluateBoard();
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
             * IGNORE THE FOLLOWING COMMENTARY, IT NEEDS TO BE UPDATED !!!
            //  * 
            //  * The score is calculated by checking the threats of the AI and the enemy
            //  * Each threat score depends on the number of stones in a row it contains
            //  * 
            //  * 1 stone in a row: 100 points
            //  * 2 stones in a row: 300 points
            //  * 3 stones in a row: 600 points
            //  * 4 stones in a row: 1000 points
            //  * 5 stones in a row is a win, so it won't be even computed
            //  * 
            //  * For enemy threats, the score is the same but negative
            //  * 1 stone in a row: -100 points
            //  * 2 stones in a row: -300 points
            //  * 3 stones in a row: -600 points
            //  * 4 stones in a row: -1000 points
            //  * 
            //  * The final score is the sum of all the threats in the board
            //  * 
            //  * Example: \n
            //  * 1. 600 : (1,1) (1,2) (1,3) \n
            //  * 2. 300 : (5,1) (5,2) \n
            //  * 3. -1000 : (2,2) (2,3) (2,4) (2,5) \n
            //  * 4. -100 : (5,5)
            //  * 
            //  * The final score will be [900 - 1100](-200). The AI is in a losing position
            //  * 
            //  * Second example: \n
            //  * 1. 600 : (1,1) (1,2) (1,3) \n
            //  * 2. 1000 : (12,2) (12,3) (12,4) (12,5) \n
            //  * 3. 1000 : (3,3) (3,4) (3,5) (3,6) \n
            //  * 4. -300 : (5,1) (5,2) \n
            //  * 5. -600 : (9,1) (9,2) (9,3) \n
            //  * 6. -600 : (7,1) (7,2) (7,3) \n
            //  * 7. -600 : (11,1) (11,2) (11,3) \n
            //  * 
            //  * The final score will be [2600 - 2100](500). The AI is in a winning position
             */
            int evaluateBoard() {
                const int WIN_SCORE = 1000000;
                const int LOSS_SCORE = -1000000;
                int score = 0;
                // linePower, color, direction, positions of the line, pattern : None, S5, D4, S4, D3, W3, S3, D2, W2, S2
                std::vector<std::tuple<int, uint8_t, uint8_t, std::array<Stone, 9>, std::string>> lines;

                for (uint8_t x = 0; x < 20; ++x) {
                    for (uint8_t y = 0; y < 20; ++y) {
                        if (board.board[x][y] != 0) {
                            uint8_t color = board.board[x][y];
                            std::array<Stone, 9> positions[4]; // 4 directions: horizontal, vertical, diagonal, anti-diagonal

                            for (int i = -4; i <= 4; ++i) {
                                int posX = x + i;
                                if (posX >= 0 && posX < 20)
                                    positions[0][i + 4] = Stone(Position(posX, y), (Color)board.board[posX][y]);
                                else
                                    positions[0][i + 4] = Stone(Position(posX, y), Color::OUT_OF_BOUND);
                            }
                            lines.push_back(std::make_tuple(0, color, 1, positions[0], "None"));

                            // Vertical line
                            for (int i = -4; i <= 4; ++i) {
                                int posY = y + i;
                                if (posY >= 0 && posY < 20)
                                    positions[1][i + 4] = Stone(Position(x, posY), (Color)board.board[x][posY]);
                                else
                                    positions[1][i + 4] = Stone(Position(x, posY), Color::OUT_OF_BOUND);
                            }
                            lines.push_back(std::make_tuple(1, color, 0, positions[1], "None"));

                            // Diagonal (\ direction)
                            for (int i = -4; i <= 4; ++i) {
                                int posX = x + i;
                                int posY = y + i;
                                if (posX >= 0 && posX < 20 && posY >= 0 && posY < 20)
                                    positions[2][i + 4] = Stone(Position(posX, posY), (Color)board.board[posX][posY]);
                                else
                                    positions[2][i + 4] = Stone(Position(posX, posY), Color::OUT_OF_BOUND);
                            }
                            lines.push_back(std::make_tuple(2, color, 2, positions[2], "None"));

                            // Anti Diagonal (/ direction)
                            for (int i = -4; i <= 4; ++i) {
                                int posX = x + i;
                                int posY = y - i;
                                if (posX >= 0 && posX < 20 && posY >= 0 && posY < 20)
                                    positions[3][i + 4] = Stone(Position(posX, posY), (Color)board.board[posX][posY]);
                                else
                                    positions[3][i + 4] = Stone(Position(posX, posY), Color::OUT_OF_BOUND);
                            }
                            lines.push_back(std::make_tuple(3, color, 3, positions[3], "None"));
                        }
                    }
                }

                for (auto &line : lines) {
                    std::get<0>(line) = checkPattern(line);
                }

                // Sort the lines by color and then power
                std::sort(lines.begin(), lines.end(), [](const auto &a, const auto &b) {
                    int colorA = std::get<1>(a);
                    int colorB = std::get<1>(b);

                    if (colorA != colorB) // First by color, AI first
                        return colorA < colorB;

                    int powerA = std::get<0>(a);
                    int powerB = std::get<0>(b);

                    if (powerA != powerB) // Then by power, descending
                        return powerA > powerB;

                    int directionA = std::get<2>(a);
                    int directionB = std::get<2>(b);

                    return directionA < directionB; // Finally by direction, alphabetical
                });

                for (auto &line : lines) {
                    int linePower = std::get<0>(line);
                    uint8_t color = std::get<1>(line);
                    uint8_t direction = std::get<2>(line);
                    std::array<Stone, 9> positions = std::get<3>(line);
                    std::string &patternStr = std::get<4>(line);

                    // Convert direction to a readable format
                    std::string directionStr;
                    switch (direction) {
                        case 0: directionStr = "Horizontal"; break;
                        case 1: directionStr = "Vertical"; break;
                        case 2: directionStr = "Diagonal \\"; break;
                        case 3: directionStr = "Anti Diagonal /"; break;
                        default: directionStr = "Unknown";
                    }

                    // Display the line information
                    std::cout << "DEBUG Line: " 
                            << (color == 1 ? "AI" : "ENEMY") 
                            << " | Dir: " << directionStr 
                            << " | Pow: " << linePower 
                            << " | Pat: " << patternStr
                            << " | Pos: ";

                    // Display each stone in the line
                    for (const auto &stone : positions) {
                        std::cout << stone.color;
                    }
                    std::cout << std::endl;
                }
                return score;
            }

            /**
             * @brief Check if there are any patterns in the line
             * 
             * @param line : the line to check for patterns
             * 
             * The line contains : 
             * - the power of the line (0 by default)
             * - the color of the line (who has the center stone)
             * - the direction of the line (0: horizontal, 1: vertical, 2: diagonal, 3: anti-diagonal)
             * - the positions of the line with their corresponding colors
             * 
             * This function will check for patterns in the line and return the power of the line
             * For the patterns below, we will consider the following board:
             * X = AI, O = ENEMY, + = EMPTY, # = OUT_OF_BOUND
             * 
             * This function, must be able to recognize the following patterns:
             * 
             * S5 : simple five, 5 stones in a row : +OXXXXXOO, +OXXXXXX+ : 5 PX stones, V0
             * 
             * Next patterns count 4 PX stones and are considered as an A1 pattern (win in 1 round, but not assured)
             * D4 : double four : +O+XXXX+O, XXX+X+XXX : 4 PX stones and 2 triggers, also called a V1 pattern (assured win in 1 round)
             * S4 : simple four : +O+XXXXOO, XXXOX+XXX : 4 PX stones and 1 trigger, not a V1 pattern
             * 
             * Next patterns count 3 PX stones and are considered as an A2 pattern (win in 2 rounds, but not assured)
             * D3 : double three : +++XXX++O, +XX+X+XX+ : Turns into a D4 pattern, very powerful, V2 pattern (assured win in 2 rounds)
             * W3 : weak three : +O+XX+X+O, O+XXX+++O : Turns either into a D4 or S4 pattern depending on the enemy input
             * S3 : simple three : +O+XXX+OO, OOXXX+++O : Turns into a S4 pattern
             * 
             * Next patterns count 2 PX stones and are considered as an A3 pattern (win in 3 rounds, but not assured)
             * D2 : double two : ++++XX++O, O++XX+++O : Turns into a D3 pattern, very powerful, V3 pattern (assured win in 3 rounds)
             * W2 : weak two : +O++XX++O, O++XX++OO : Turns into a W3 pattern
             * S2 : simple two : +O++XX+OO, OO+XX++OO : Turns into a S3 pattern, very weak pattern
             * 
             * No need to check for A4 patterns, as they are not useful
             */
            int checkPattern(std::tuple<int, uint8_t, uint8_t, std::array<Stone, 9>, std::string> &line) {
                // linePower, color, direction, positions of the line
                uint8_t color = std::get<1>(line);
                uint8_t direction = std::get<2>(line);
                std::array<Stone, 9> positions = std::get<3>(line);
                int dx = 0;
                int dy = 0;
                int idx = 0;
                std::get<4>(line) = "None";

                // Compute dx and dy based on the direction
                switch (direction) {
                    case 0: dy = 1; break;
                    case 1: dx = 1; break;
                    case 2: dx = 1; dy = 1; break;
                    case 3: dx = -1; dy = 1; break;
                }

                // Start the pattern recognition...

                // Check for S5 patterns
                idx = 0;
                for (const auto &stone : positions) {
                    if (stone.color != (Color)color) {
                        idx++;
                        continue;
                    }

                    // Check if the stone idx is smaller than 5 because we can't have
                    // a S5 pattern without 5 positions to explore
                    if (idx >= 5) {
                        idx++;
                        continue;
                    }

                    // Check if there are 5 stones in a row
                    if (checkNInRow(stone.pos.x, stone.pos.y, dx, dy, color, 5)) {
                        std::cout << "DEBUG Found a S5 pattern for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
                        std::get<4>(line) = "S5";
                        return 1000000;
                    }
                }

                // Check for D4 patterns
                idx = 0;
                for (const auto &stone : positions) {
                    if (stone.color != (Color)color) {
                        idx++;
                        continue;
                    }
                    bool fourInRow = checkNInRow(stone.pos.x, stone.pos.y, dx, dy, color, 4);
                    bool beforeEmpty = isEmpty(stone.pos.x - dx, stone.pos.y - dy);
                    bool afterEmpty = isEmpty(stone.pos.x + 4 * dx, stone.pos.y + 4 * dy);

                    // Simple case where all the stones are in a row
                    if (fourInRow && beforeEmpty && afterEmpty) {
                        std::cout << "DEBUG Found a D4 pattern for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
                        std::get<4>(line) = "D4";
                        return 100000;
                    }

                    // Check if the stone is the first of the line
                    if (idx != 0) {
                        idx++;
                        continue;
                    }

                    // Case where the stones have a trigger but not at the end or the beginning
                    bool fourInRowWithTrigger = checkNInRowWithTTriggers(stone.pos.x, stone.pos.y, dx, dy, color, 9, 2);
                    bool lastStoneEmpty = isEmpty(stone.pos.x + 8 * dx, stone.pos.y + 8 * dy);
                    if (fourInRowWithTrigger && !lastStoneEmpty) {
                        std::cout << "DEBUG Found a D4 pattern with a trigger for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
                        std::get<4>(line) = "D4 Trigger";
                        return 100000;
                    }
                    idx++;
                }

                // Check for S4 patterns
                idx = 0;
                for (const auto &stone : positions) {
                    if (stone.color != (Color)color) {
                        idx++;
                        continue;
                    }
                    bool fourInRow = checkNInRow(stone.pos.x, stone.pos.y, dx, dy, color, 4);
                    bool beforeEmpty = isEmpty(stone.pos.x - dx, stone.pos.y - dy);
                    bool afterEmpty = isEmpty(stone.pos.x + 4 * dx, stone.pos.y + 4 * dy);
                    if ((fourInRow && beforeEmpty) || (fourInRow && afterEmpty)) {
                        std::cout << "DEBUG Found a S4 pattern for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
                        std::get<4>(line) = "S4";
                        return 10000;
                    }

                    // Check if the stone is smaller than 5 because we can't have a S4 pattern with a trigger
                    // when there are only 4 positions left to explore
                    if (idx >= 5) {
                        idx++;
                        continue;
                    }

                    // Case where the stones have a trigger but not at the end or the beginning
                    bool fourInRowWithTrigger = checkNInRowWithTTriggers(stone.pos.x, stone.pos.y, dx, dy, color, 5, 1);
                    if (fourInRowWithTrigger) {
                        std::cout << "DEBUG Found a S4 pattern with a trigger for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
                        std::get<4>(line) = "S4 Trigger";
                        return 10000;
                    }
                    idx++;
                }

                // Check for D3 patterns
                idx = 0;
                for (const auto &stone : positions) {
                    if (stone.color != (Color)color) {
                        idx++;
                        continue;
                    }
                    bool threeInRow = checkNInRow(stone.pos.x, stone.pos.y, dx, dy, color, 3);
                    bool beforeEmpty =
                        isEmpty(stone.pos.x - dx, stone.pos.y - dy) &&
                        isEmpty(stone.pos.x - 2 * dx, stone.pos.y - 2 * dy);
                    bool afterEmpty =
                        isEmpty(stone.pos.x + 3 * dx, stone.pos.y + 3 * dy) &&
                        isEmpty(stone.pos.x + 4 * dx, stone.pos.y + 4 * dy);
                    
                    // Simple case where 3 stones are in a row and there are two empty spaces on each side
                    if ((threeInRow && beforeEmpty && afterEmpty)) {
                        std::cout << "DEBUG Found a D3 pattern for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
                        std::get<4>(line) = "D3";
                        return 1000;
                    }

                    // Check if the stone is the first of the line
                    if (idx != 0) {
                        idx++;
                        continue;
                    }

                    // Case where the stones have a trigger but not at the end or the beginning
                    bool threeInRowWithTrigger = checkNInRowWithTTriggers(stone.pos.x, stone.pos.y, dx, dy, color, 9, 4);
                    if (threeInRowWithTrigger) {
                        std::cout << "DEBUG Found a D3 pattern with a trigger for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
                        std::get<4>(line) = "D3 Trigger";
                        return 1000;
                    }
                    idx++;
                }

                // Check for W3 patterns
                idx = 0;
                for (const auto &stone : positions) {
                    if (stone.color != (Color)color) {
                        idx++;
                        continue;
                    }
                    bool threeInRow = checkNInRow(stone.pos.x, stone.pos.y, dx, dy, color, 3);
                    bool beforeEmpty =
                        isEmpty(stone.pos.x - dx, stone.pos.y - dy) &&
                        isEmpty(stone.pos.x - 2 * dx, stone.pos.y - 2 * dy);
                    bool afterEmpty =
                        isEmpty(stone.pos.x + 3 * dx, stone.pos.y + 3 * dy) &&
                        isEmpty(stone.pos.x + 4 * dx, stone.pos.y + 4 * dy);
                    
                    // Simple case where 3 stones are in a row and there is one side where
                    // there are two empty spaces and the other side has one empty space
                    if ((threeInRow && beforeEmpty && isEmpty(stone.pos.x + 3 * dx, stone.pos.y + 3 * dy)) ||
                        (threeInRow && afterEmpty && isEmpty(stone.pos.x - dx, stone.pos.y - dy))) {
                        std::cout << "DEBUG Found a W3 pattern for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
                        std::get<4>(line) = "W3";
                        return 500;
                    }

                    // We need at least 5 positions to check for a trigger,
                    // so we skip the last 4 positions
                    if (idx >= 5) {
                        idx++;
                        continue;
                    }

                    beforeEmpty = isEmpty(stone.pos.x - dx, stone.pos.y - dy);
                    afterEmpty = isEmpty(stone.pos.x + 4 * dx, stone.pos.y + 4 * dy);

                    bool threeInRowWithTrigger = checkNInRowWithTTriggers(stone.pos.x, stone.pos.y, dx, dy, color, 4, 1);
                    if (threeInRowWithTrigger && beforeEmpty && afterEmpty) {
                        std::cout << "DEBUG Found a W3 pattern with a trigger for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
                        std::get<4>(line) = "W3 Trigger";
                        return 500;
                    }
                    idx++;
                }

                // Check for S3 patterns
                idx = 0;
                for (const auto &stone : positions) {
                    if (stone.color != (Color)color) {
                        idx++;
                        continue;
                    }
                    bool threeInRow = checkNInRow(stone.pos.x, stone.pos.y, dx, dy, color, 3);
                    bool beforeEmpty =
                        isEmpty(stone.pos.x - dx, stone.pos.y - dy) &&
                        isEmpty(stone.pos.x - 2 * dx, stone.pos.y - 2 * dy);
                    bool afterEmpty =
                        isEmpty(stone.pos.x + 3 * dx, stone.pos.y + 3 * dy) &&
                        isEmpty(stone.pos.x + 4 * dx, stone.pos.y + 4 * dy);
                    
                    // Simple case where 3 stones are in a row and there are two empty spaces on each side
                    if (((threeInRow && beforeEmpty) || (threeInRow && afterEmpty))) {
                        std::cout << "DEBUG Found a S3 pattern for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
                        std::get<4>(line) = "S3";
                        return 100;
                    }

                    // Check if the stone is the first of the line
                    if (idx >= 5) {
                        idx++;
                        continue;
                    }

                    beforeEmpty = isEmpty(stone.pos.x - dx, stone.pos.y - dy);
                    afterEmpty = isEmpty(stone.pos.x + 4 * dx, stone.pos.y + 4 * dy);

                    // Case where the stones have a trigger but not at the end or the beginning
                    bool threeInRowWithTrigger = checkNInRowWithTTriggers(stone.pos.x, stone.pos.y, dx, dy, color, 4, 1);
                    if ((threeInRowWithTrigger && beforeEmpty) || (threeInRowWithTrigger && afterEmpty)) {
                        std::cout << "DEBUG Found a S3 pattern with a trigger for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
                        std::get<4>(line) = "S3 Trigger";
                        return 100;
                    }
                    idx++;
                }

                // Check for D2 patterns
                // Check for W2 patterns
                // Check for S2 patterns

                return 0;
            }

            bool isEmpty(int x, int y) {
                return x >= 0 && x < 20 && y >= 0 && y < 20 && board.board[x][y] == 0;
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
                int direction = 0; // 0: horizontal, 1: vertical, 2: diagonal, 3: anti-diagonal

                for (int i = 1; i < 6; ++i) {
                    if ((y <= 20 - i && checkNInRow(x, y, 0, 1, color, i))) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x, y + i - 1));
                        direction = 1;
                    } else if (x <= 20 - i && checkNInRow(x, y, 1, 0, color, i)) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x + i - 1, y));
                        direction = 0;
                    } else if (x <= 20 - i && y <= 20 - i && checkNInRow(x, y, 1, 1, color, i)) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x + i - 1, y + i - 1));
                        direction = 2;
                    } else if (x >= i && y <= 20 - i && checkNInRow(x, y, -1, 1, color, i)) {
                        threatScore += 100 * i * colorEval;
                        threatStones.push_back(Position(x - i, y + i - 1));
                        direction = 3;
                    } else
                        break;
                }
                if (threatStones.size() > 0) {
                    __threats.push_back(std::make_pair(threatScore, threatStones));
                }
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

            bool checkNInRow(int x, int y, int dx, int dy, uint8_t color, int n) {
                for (int i = 0; i < n; ++i) {
                    int newX = x + i * dx;
                    int newY = y + i * dy;

                    if (newX < 0 || newX >= 20 || newY < 0 || newY >= 20)
                        return false;

                    if (board.board[newX][newY] != color) {
                        return false;
                    }
                }
                return true;
            }

            bool checkNInRowWithTTriggers(int x, int y, int dx, int dy, uint8_t color, int n, int t) {
                int tFound = 0;
                int nFound = 0;

                for (int i = 0; i < n; ++i) {
                    int newX = x + i * dx;
                    int newY = y + i * dy;

                    if (newX < 0 || newX >= 20 || newY < 0 || newY >= 20)
                        return false;

                    if (board.board[newX][newY] == (uint8_t)Color::EMPTY) {
                        tFound++;
                    } else if (board.board[newX][newY] == color) {
                        nFound++;
                    }
                }
                // std::cout << "DEBUG nFound: " << nFound << " tFound: " << tFound << std::endl;
                if (nFound == n - t && tFound == t)
                    return true;
                return false;
            }

            std::pair<uint8_t, uint8_t> getBestMove() {
                int bestScore = std::numeric_limits<int>::min();
                std::pair<uint8_t, uint8_t> bestMove = std::make_pair(0, 0);

                for (uint8_t x = 0; x < 20; ++x) {
                    for (uint8_t y = 0; y < 20; ++y) {
                        if (board.board[x][y] == 0) {
                            //
                        }
                    }
                }
                return bestMove;
            }

    };
};

#endif // AI_HPP
