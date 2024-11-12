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

                Position bestMove = getBestMove();
                uint8_t x = bestMove.x;
                uint8_t y = bestMove.y;

                board.board[x][y] = 1;

                addToSearchBoard(x, y, 1);
                std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << std::endl;
                int status = evaluateBoard();
                std::cout << (int)x << "," << (int)y << std::endl;
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
                searchBoard.board[x][y] = color + 3;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if (x + i >= 0 && x + i < 20 && y + j >= 0 && y + j < 20 && searchBoard.board[x + i][y + j] == 0)
                            searchBoard.board[x + i][y + j] = 3;
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
                        switch (searchBoard.board[i][j]) {
                            case 0:
                                // Color gray
                                std::cout << "\033[90m+  \033[0m"; // Gray
                                break;
                            case 1:
                                // Color red
                                std::cout << "\033[91mX  \033[0m"; // Red
                                break;
                            case 2:
                                // Color blue
                                std::cout << "\033[94mO  \033[0m"; // Blue
                                break;
                            case 3:
                                // Color white
                                std::cout << "\033[97m#  \033[0m"; // White
                                break;
                            case 4:
                                // Color green
                                std::cout << "\033[92mX  \033[0m"; // Green
                                searchBoard.board[i][j] = 1;
                                break;
                            case 5:
                                // Color green
                                std::cout << "\033[92mO  \033[0m"; // Green
                                searchBoard.board[i][j] = 2;
                                break;
                        }
                    }
                    std::cout << std::endl;
                }
            }

            // Attributes
            Board board;
            Board searchBoard; // Search board contains each cell to be evaluated

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

                // Get the total score of the board
                for (auto &line : lines) {
                    int colorMultiplier = std::get<1>(line) == 1 ? 1 : -1;
                    score += std::get<0>(line) * colorMultiplier;
                }

                // std::cout << "DEBUG Score: " << score << std::endl;

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

                // for (auto &line : lines) {
                //     int linePower = std::get<0>(line);
                //     uint8_t color = std::get<1>(line);
                //     uint8_t direction = std::get<2>(line);
                //     std::array<Stone, 9> positions = std::get<3>(line);
                //     std::string &patternStr = std::get<4>(line);

                //     // Convert direction to a readable format
                //     std::string directionStr;
                //     switch (direction) {
                //         case 0: directionStr = "Horizontal"; break;
                //         case 1: directionStr = "Vertical"; break;
                //         case 2: directionStr = "Diagonal \\"; break;
                //         case 3: directionStr = "Anti Diagonal /"; break;
                //         default: directionStr = "Unknown";
                //     }

                //     // Display the line information
                //     std::cout << "DEBUG Line: "
                //             << (color == 1 ? "AI" : "ENEMY")
                //             << " | Dir: " << directionStr
                //             << " | Pow: " << linePower
                //             << " | Pat: " << patternStr
                //             << " | Pos: ";

                //     // Display each stone in the line
                //     for (const auto &stone : positions) {
                //         std::cout << stone.color;
                //     }
                //     std::cout << std::endl;
                // }
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
                        // std::cout << "DEBUG Found a S5 pattern for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a D4 pattern for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a D4 pattern with a trigger for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a S4 pattern for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a S4 pattern with a trigger for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a D3 pattern for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a D3 pattern with a trigger for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a W3 pattern for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a W3 pattern with a trigger for: " << (color == 1 ? "AI" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a S3 pattern for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
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
                        // std::cout << "DEBUG Found a S3 pattern with a trigger for: " << (color == 1 ? "Player" : "Enemy") << std::endl;
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

            /**
             * @brief Get the best move for the AI
             *
             * @return Position : the best move for the AI
             */
            Position getBestMove() {
                int bestScore = std::numeric_limits<int>::min();
                Position bestMove;

                for (uint8_t x = 0; x < 20; ++x) {
                    for (uint8_t y = 0; y < 20; ++y) {
                        if (searchBoard.board[x][y] != 3)
                            continue;
                        board.board[x][y] = 1;
                        // Simulate enemy move
                        for (uint8_t x2 = 0; x2 < 20; ++x2) {
                            for (uint8_t y2 = 0; y2 < 20; ++y2) {
                                if (searchBoard.board[x2][y2] != 3)
                                    continue;
                                board.board[x2][y2] = 2;
                                int score = evaluateBoard();
                                if (score > bestScore) {
                                    bestScore = score;
                                    bestMove = Position(x, y);
                                }
                                board.board[x2][y2] = 0;
                            }
                        }
                        board.board[x][y] = 0;
                    }
                }
                if (bestScore == std::numeric_limits<int>::min()) {
                    while (true) {
                        uint8_t x = rand() % 20;
                        uint8_t y = rand() % 20;
                        if (board.board[x][y] == 0) {
                            bestMove = Position(x, y);
                            break;
                        }
                    }
                }
                std::cout << "DEBUG Best move found: " << (int)bestMove.x << "," << (int)bestMove.y << " with score: " << bestScore << std::endl;
                return bestMove;
            }
    };
};

#endif // AI_HPP
