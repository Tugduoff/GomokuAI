/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Board Class
*/

#include <array>
#include <limits>
#include <unordered_map>
#include <mutex>
#include <future>
#include "Board.hpp"

void Gomoku::Board::initLines()
{
    for (uint8_t lineI = 0; lineI < 20; ++lineI) {
        for (uint8_t lineJ = 0; lineJ < 20; ++lineJ) {
            Position pos(lineI, lineJ);
            // Add the new lines and evaluate their score

            std::array<Stone, 9> positions[4]; // 4 directions: horizontal, vertical, diagonal, anti-diagonal

            for (int i = -4; i <= 4; ++i) {
                int posX = pos.x + i;
                if (posX >= 0 && posX < 20)
                    positions[0][i + 4] = Stone(Position(posX, pos.y), (Color)board[posX][pos.y]);
                else
                    positions[0][i + 4] = Stone(Position(posX, pos.y), Color::OUT_OF_BOUND);
            }

            // Vertical line
            for (int i = -4; i <= 4; ++i) {
                int posY = pos.y + i;
                if (posY >= 0 && posY < 20)
                    positions[1][i + 4] = Stone(Position(pos.x, posY), (Color)board[pos.x][posY]);
                else
                    positions[1][i + 4] = Stone(Position(pos.x, posY), Color::OUT_OF_BOUND);
            }

            // Diagonal (\ direction)
            for (int i = -4; i <= 4; ++i) {
                int posX = pos.x + i;
                int posY = pos.y + i;
                if (posX >= 0 && posX < 20 && posY >= 0 && posY < 20)
                    positions[2][i + 4] = Stone(Position(posX, posY), (Color)board[posX][posY]);
                else
                    positions[2][i + 4] = Stone(Position(posX, posY), Color::OUT_OF_BOUND);
            }

            // Anti Diagonal (/ direction)
            for (int i = -4; i <= 4; ++i) {
                int posX = pos.x + i;
                int posY = pos.y - i;
                if (posX >= 0 && posX < 20 && posY >= 0 && posY < 20)
                    positions[3][i + 4] = Stone(Position(posX, posY), (Color)board[posX][posY]);
                else
                    positions[3][i + 4] = Stone(Position(posX, posY), Color::OUT_OF_BOUND);
            }
            std::array<Line, 4> newLines;

            boardLines[lineI * 20 + lineJ][0] = Line(0, 1, 0, (Color)board[lineI][lineJ], std::move(positions[0]));
            boardLines[lineI * 20 + lineJ][1] = Line(0, 0, 1, (Color)board[lineI][lineJ], std::move(positions[1]));
            boardLines[lineI * 20 + lineJ][2] = Line(0, 1, 1, (Color)board[lineI][lineJ], std::move(positions[2]));
            boardLines[lineI * 20 + lineJ][3] = Line(0, 1, -1, (Color)board[lineI][lineJ], std::move(positions[3]));
        }
    }
}

void Gomoku::Board::playMove(const Position &pos, Color color)
{
    // Store old colors for each 9 cells
    std::array<Stone, 9> oldPositions;

    // Store the center position explicitly
    oldPositions[4] = Stone(pos, board[pos.x][pos.y]);
    board[pos.x][pos.y] = color; // Play the move

    // Iterate through the 3x3 grid centered at (x, y)
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0)
                continue;
            int idx = (i + 1) * 3 + (j + 1); // 3x3 grid index calculation
            Position stonePos(pos.x + i, pos.y + j);

            if (isOutOfBound(stonePos))
                oldPositions[idx] = Stone(stonePos, Color::OUT_OF_BOUND);
            else if (board[stonePos.x][stonePos.y] == Color::EMPTY) {
                oldPositions[idx] = Stone(stonePos, Color::EMPTY);
                board[stonePos.x][stonePos.y] = Color::TO_EXPLORE;
            } else
                oldPositions[idx] = Stone(stonePos, board[stonePos.x][stonePos.y]);
        }
    }

    boardMoves.push_back(oldPositions);

    uniqueLines.clear();

    for (auto &lineArr : boardLines) {
        for (auto &line : lineArr) {
            for (auto &stone : line.positions) {
                if (stone.pos == pos) {
                    if (line.positions[4].pos == pos) {
                        line.score = 0;
                        line.color = color; // Change line color to the new stone color
                        line.dirty = false;
                    }
                    stone.color = color;
                    if (line.color == Color::EMPTY)
                        break;
                    line.dirty = true;
                    break; // Only one stone per line can be the new stone
                }
            }
            if (line.dirty) {
                line.dirty = false;
                if (line.color == Color::EMPTY)
                    continue;
                line.score = checkPattern(line);
            }
            if (line.score != 0) {
                bool isDuplicate = false;
                for (auto &uline : uniqueLines) {
                    if (uline == line) {
                        isDuplicate = true;
                        break;
                    }
                }
                if (!isDuplicate) {
                    uniqueLines.push_back(line);
                }
            }
        }
    }

    status = ONGOING;
    for (auto &line : uniqueLines) {
        if (line.score == std::numeric_limits<int>::max())
            status = WIN;
        if (line.score == std::numeric_limits<int>::min())
            status = LOSE;
    }
}

void Gomoku::Board::undoMove(const Position &pos)
{
    // Retrieve the last stored state
    std::array<Stone, 9> oldPositions = boardMoves.back();
    boardMoves.pop_back(); // Remove the last element from the stack
    // std::cout << "DEBUG | Removed from search board: " << boardMoves.size() << std::endl;

    // Iterate through the 3x3 grid centered at (x, y)
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int idx = (i + 1) * 3 + (j + 1); // 3x3 grid index calculation
            const Stone &stone = oldPositions[idx];

            if (stone.color != Color::OUT_OF_BOUND)
                board[stone.pos.x][stone.pos.y] = stone.color;
        }
    }

    // Update all lines containing the new stone and evaluate their score
    for (auto &lineArr : boardLines) {
        for (auto &line : lineArr) {
            for (auto &stone : line.positions) {
                if (stone.pos == pos) {
                    line.dirty = true;
                    if (stone == line.positions[4]) {
                        line.score = 0;
                        line.color = Color::EMPTY;
                        line.dirty = false;
                    }
                    stone.color = board[pos.x][pos.y] == Color::TO_EXPLORE ? Color::EMPTY : board[pos.x][pos.y];
                }
            }
            if (line.color == Color::EMPTY)
                continue;
            if (line.dirty) {
                line.dirty = false;
                line.score = checkPattern(line);
            }
        }
    }
}

int Gomoku::Board::checkPattern(Line &line)
{
    // Empty the line triggers
    // Start the pattern recognition...

    std::unordered_map<Position, bool> emptyCache; // Caching
    auto isPosEmptyCached = [&](const Position& pos) {
        if (emptyCache.find(pos) == emptyCache.end())
            emptyCache[pos] = isPosEmpty(pos);
        return emptyCache[pos];
    };

    int idx = -1;
    for (const auto &stone : line.positions) {
        idx++;
        if (stone.color != line.color || idx > 7)
            continue;

        // Check for S5 pattern : XXXXX : max ....XXXXX
        if (checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 5) && idx <= 4) {
            if (line.color == Color::ENEMY)
                return std::numeric_limits<int>::min();
            return std::numeric_limits<int>::max();
        }
        bool fourInRow = checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 4);
        // Check for D4 pattern : +XXXX+ : max ....XXXX+
        if (isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x + 4 * line.dx, stone.pos.y + 4 * line.dy)) &&
            fourInRow && idx <= 4) {
            // Fill the triggers
            if (line.color == Color::ENEMY)
                return individual.D4_pattern_pl;
            return individual.D4_pattern_ai;
        }
        // Check for S4 pattern : +XXXX : max .....XXXX
        if (isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) && fourInRow && idx <= 5) {
            if (line.color == Color::ENEMY)
                return individual.S4_pattern_pl;
            return individual.S4_pattern_ai;
        }
        // Check for S4 patterns : X+XXX, XX+XX, XXX+X, XXXX+ : max ....X+XXX, ....XX+XX, ....XXX+X, ....XXXX+
        if (idx <= 4 && checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 1)) {
            if (line.color == Color::ENEMY)
                return individual.S4_pattern_pl;
            return individual.S4_pattern_ai;
        }
        // Check for D3 pattern : ++XXX+ & S3 pattern : ++XXX : max .....XXX+ & ......XXX
        if (checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 3) &&
            isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) && idx <= 6) {
            // D3 pattern
            if (isPosEmptyCached(Position(stone.pos.x + 3 * line.dx, stone.pos.y + 3 * line.dy)) && idx <= 5) {
                if (line.color == Color::ENEMY)
                    return individual.D3_pattern_pl;
                return individual.D3_pattern_ai;
            }
            // S3 pattern
            if (line.color == Color::ENEMY)
                return individual.S3_pattern_pl;
            return individual.S3_pattern_ai;
        }
        // Check for D3 patterns : +XXX++, +XX+X+, +X+XX+ : max ....XXX++, ....XX+X+, ....X+XX+
        if (idx <= 4 && isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x + 5 * line.dx, stone.pos.y + 4 * line.dy)) &&
            checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 2)) {
            if (line.color == Color::ENEMY)
                return individual.D3_pattern_pl;
            return individual.D3_pattern_ai;
        }
        // Check for S3 patterns : +XXX+, +X+XX, +XX+X : max .....XXX+, .....X+XX, .....XX+X
        if (idx <= 5 && checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 4, 1) &&
            isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy))) {
            if (line.color == Color::ENEMY)
                return individual.S3_pattern_pl;
            return individual.S3_pattern_ai;
        }
        // Check for S3 patterns : XXX++, XX++X, X++XX, X+XX+, X+X+X, XX+X+ : max ....XXX++, ....XX++X, ....X++XX, ....X+XX+, ....X+X+X, ....XX+X+
        if (idx <= 4 && checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 2)) {
            if (line.color == Color::ENEMY)
                return individual.S3_pattern_pl;
            return individual.S3_pattern_ai;
        }
        // Check for D2 pattern : +++XX+ : max ......XX+
        if (idx <= 6 && isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x - 3 * line.dx, stone.pos.y - 3 * line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x + 2 * line.dx, stone.pos.y + 2 * line.dy)) &&
            checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 2)) {
            if (line.color == Color::ENEMY)
                return individual.D2_pattern_pl;
            return individual.D2_pattern_ai;
        }
        // Check for D2 patterns : ++XX++, ++X+X+ : max .....XX++, .....X+X+
        if (idx <= 5 && isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x + 5 * line.dx, stone.pos.y + 5 * line.dy)) &&
            checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 4, 2)) {
            if (line.color == Color::ENEMY)
                return individual.D2_pattern_pl;
            return individual.D2_pattern_ai;
        }
        // Check for D2 patterns : +XX+++, +X+X++, +X++X+ : max ....XX+++, ....X+X++, ....X++X+
        if (idx <= 4 && isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x + 5 * line.dx, stone.pos.y + 5 * line.dy)) &&
            checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 3)) {
            if (line.color == Color::ENEMY)
                return individual.D2_pattern_pl;
            return individual.D2_pattern_ai;
        }
        // Check for S2 pattern : +++XX : max .......XX
        if (idx <= 7 && checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 2) &&
            isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x - 3 * line.dx, stone.pos.y - 3 * line.dy))) {
            if (line.color == Color::ENEMY)
                return individual.S2_pattern_pl;
            return individual.S2_pattern_ai;
        }
        // Check for S2 patterns : ++XX+, ++X+X : max ......XX+, ......X+X
        if (idx <= 6 && isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmptyCached(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) &&
            checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 3, 1)) {
            if (line.color == Color::ENEMY)
                return individual.S2_pattern_pl;
            return individual.S2_pattern_ai;
        }
        // Check for S2 patterns : +XX++, +X+X+, +X++X : max .....XX++, .....X+X+, .....X++X
        if (idx <= 5 && isPosEmptyCached(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 4, 2)) {
            if (line.color == Color::ENEMY)
                return individual.S2_pattern_pl;
            return individual.S2_pattern_ai;
        }
        // Check for S2 patterns : XX+++, X+++X, X++X+, X+X++
        if (idx <= 4 && checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 3)) {
            if (line.color == Color::ENEMY)
                return individual.S2_pattern_pl;
            return individual.S2_pattern_ai;
        }
    }
    return 0;
}

bool Gomoku::Board::isPosEmpty(const Position &pos)
{
    return !isOutOfBound(pos) && (getAt(pos) == Color::EMPTY || getAt(pos) == Color::TO_EXPLORE);
}

bool Gomoku::Board::isOutOfBound(const Position &pos)
{
    return pos.x >= 20 || pos.y >= 20;
}

const Gomoku::Color &Gomoku::Board::getAt(const Position &pos)
{
    return board[pos.x][pos.y];
}

bool Gomoku::Board::checkNColorInRow(const Position &pos, int dx, int dy, const Color &color, int n)
{
    for (uint8_t i = 0; i < n; ++i) {
        if (getAt(Position(pos.x + i * dx, pos.y + i * dy)) != color)
            return false;
    }
    return true;
} 

bool Gomoku::Board::checkNColorInRowWithTTriggers(const Position &pos, int dx, int dy, const Color &color, int n, int t)
{
    int tFound = 0;
    int nFound = 0;

    if (isOutOfBound(pos) || isOutOfBound(Position(pos.x + (n - 1) * dx, pos.y + (n - 1) * dy)))
        return false;

    for (int i = 0; i < n; ++i) {
        if (isPosEmpty(Position(pos.x + i * dx, pos.y + i * dy))) {
            tFound++;
            if (tFound > t) {
                return false;
            }
        } else if (getAt(Position(pos.x + i * dx, pos.y + i * dy)) == color) {
            nFound++;
        }
    }

    return (nFound == n - t && tFound == t);
}

int Gomoku::Board::count(const Color &color)
{
    int count = 0;

    for (uint8_t i = 0; i < 20; ++i) {
        for (uint8_t j = 0; j < 20; ++j) {
            if (board[i][j] == color)
                count++;
        }
    }
    return count;
}

std::ostream &Gomoku::operator<<(std::ostream &os, const Gomoku::Line &line)
{
    os << "Line: " << (line.color == Color::AI ? "AI" : (line.color == Color::ENEMY ? "ENEMY" : "EMPTY")) << " | Dir: " <<
        (int)line.dx << "," << (int)line.dy << " | Score: " << line.score << " | Pos: ";
    for (const auto &stone : line.positions) {
        os << stone.color;
    }
    os << std::endl;
    return os;
}

std::ostream &Gomoku::operator<<(std::ostream &os, const Gomoku::Color &color)
{
    switch (color) {
        case Color::AI:
            os << "\033[91mX\033[0m"; break;
        case Color::ENEMY:
            os << "\033[94mO\033[0m"; break;
        case Color::OUT_OF_BOUND:
            os << "\033[97m@\033[0m"; break;
        case Color::TO_EXPLORE:
            os << "\033[97m#\033[0m"; break;
        case Color::EMPTY:
            os << "\033[90m+\033[0m"; break;
        default:
            os << "\033[91m*\033[0m";
    }
    return os;
}
