/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Board Class
*/

#include <array>
#include <limits>
#include "Board.hpp"

void Gomoku::Board::playMove(const Position &pos, Color color)
{
    board[pos.x][pos.y] = color; // Play the move

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

    newLines[0] = Line(0, 1, 0, (Color)color, std::move(positions[0]));
    newLines[1] = Line(0, 0, 1, (Color)color, std::move(positions[1]));
    newLines[2] = Line(0, 1, 1, (Color)color, std::move(positions[2]));
    newLines[3] = Line(0, 1, -1, (Color)color, std::move(positions[3]));

    for (auto &line : newLines)
        line.score = checkPattern(line) * (color == Color::AI ? 1 : -1);

    // Update all lines containing the new stone and evaluate their score
    for (auto &line : lines) {
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 9; ++j) {
                if (line[i].positions[j].pos == pos) {
                    line[i].positions[j].color = color;
                    line[i].score = checkPattern(line[i]) * (line[i].color == Color::AI ? 1 : -1);
                }
            }
        }
    }

    lines.push_back(newLines);
}

void Gomoku::Board::undoMove(const Position &pos)
{
    board[pos.x][pos.y] = Color::EMPTY; // Undo the move

    // Remove the new lines
    lines.pop_back();

    // Update all lines containing the new stone and evaluate their score
    for (auto &line : lines) {
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 9; ++j) {
                if (line[i].positions[j].pos == pos) {
                    line[i].positions[j].color = Color::EMPTY;
                    line[i].score = checkPattern(line[i]) * (line[i].color == Color::AI ? 1 : -1);
                }
            }
        }
    }
}

int Gomoku::Board::checkPattern(Line &line)
{
    // Start the pattern recognition...
    for (const auto &stone : line.positions) {
        if (stone.color != line.color)
            continue;

        // Check for S5 pattern : XXXXX
        if (checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 5)) {
            std::cout << "DEBUG S5" << std::endl;
            return std::numeric_limits<int>::max();
        }
        bool fourInRow = checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 4);
        // Check for D4 pattern : +XXXX+
        if (fourInRow &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x + 4 * line.dx, stone.pos.y + 4 * line.dy))) {
            return 1000000;
        }
        // Check for S4 pattern : +XXXX
        if (fourInRow && isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy))) {
            if (line.color == Color::ENEMY)
                return 2000000;
            return 100000;
        }
        // Check for S4 patterns : X+XXX, XX+XX, XXX+X, XXXX+
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 1)) {
            if (line.color == Color::ENEMY)
                return 2000000;
            return 100000;
        }
        // Check for D3 pattern : ++XXX+
        if (checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 3) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy))) {
                if (isPosEmpty(Position(stone.pos.x + 3 * line.dx, stone.pos.y + 3 * line.dy))) {
                    if (line.color == Color::ENEMY)
                        return 100001;
                    return 25000;
                }
            if (line.color == Color::ENEMY)
                return 10001;
            return 10000;
        }
        // Check for D3 patterns : +XXX++, +XX+X+, +X+XX+
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 2) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x + 5 * line.dx, stone.pos.y + 5 * line.dy))) {
            if (line.color == Color::ENEMY)
                return 100001;
            return 25000;
        }
        // Check for S3 patterns : +XXX+, +X+XX, +XX+X
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 4, 1) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy))) {
            if (line.color == Color::ENEMY)
                return 10001;
            return 10000;
        }
        // Check for S3 patterns : XXX++, XX++X, X++XX, X+XX+, X+X+X, XX+X+
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 2)) {
            if (line.color == Color::ENEMY)
                return 10001;
            return 10000;
        }
        // Check for D2 pattern : +++XX+
        if (checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 2) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) &&
            isPosEmpty(Position(stone.pos.x - 3 * line.dx, stone.pos.y - 3 * line.dy)) &&
            isPosEmpty(Position(stone.pos.x + 2 * line.dx, stone.pos.y + 2 * line.dy))) {
            if (line.color == Color::ENEMY)
                return 4001;
            return 1000;
        }
        // Check for D2 patterns : ++XX++, ++X+X+
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 4, 2) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) &&
            isPosEmpty(Position(stone.pos.x + 5 * line.dx, stone.pos.y + 5 * line.dy))) {
            if (line.color == Color::ENEMY)
                return 4001;
            return 1000;
        }
        // Check for D2 patterns : +XX+++, +X+X++, +X++X+
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 3) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x + 5 * line.dx, stone.pos.y + 5 * line.dy))) {
            if (line.color == Color::ENEMY)
                return 4001;
            return 1000;
        }
        // Check for S2 pattern : +++XX
        if (checkNColorInRow(stone.pos, line.dx, line.dy, line.color, 2) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy)) &&
            isPosEmpty(Position(stone.pos.x - 3 * line.dx, stone.pos.y - 3 * line.dy))) {
            if (line.color == Color::ENEMY)
                return 101;
            return 100;
        }
        // Check for S2 patterns : ++XX+, ++X+X
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 3, 1) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy)) &&
            isPosEmpty(Position(stone.pos.x - 2 * line.dx, stone.pos.y - 2 * line.dy))) {
            if (line.color == Color::ENEMY)
                return 101;
            return 100;
        }
        // Check for S2 patterns : +XX++, +X+X+, +X++X
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 4, 2) &&
            isPosEmpty(Position(stone.pos.x - line.dx, stone.pos.y - line.dy))) {
            if (line.color == Color::ENEMY)
                return 101;
            return 100;
        }
        // Check for S2 patterns : XX+++, X+++X, X++X+, X+X++
        if (checkNColorInRowWithTTriggers(stone.pos, line.dx, line.dy, line.color, 5, 3)) {
            if (line.color == Color::ENEMY)
                return 101;
            return 100;
        }
    }
    return 0;
}

bool Gomoku::Board::isPosEmpty(const Position &pos)
{
    return !isOutOfBound(pos) && getAt(pos) == Color::EMPTY;
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
            if (tFound > t)
                return false;
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
    os << "Line: " << (line.color == Color::AI ? "AI" : "ENEMY") << " | Dir: " <<
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
