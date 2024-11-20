/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** AI
*/

#include "Algo.hpp"
#include "AI.hpp"

void Gomoku::AI::turn()
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

    Position bestMove = getBestMove();
    uint8_t x = bestMove.x;
    uint8_t y = bestMove.y;

    board.playMove(bestMove, Color::AI);

    status = evaluateBoard();
    addToSearchBoard(x, y, 1);
    displaySearchBoard();

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
    std::cout << (int)x << "," << (int)y << std::endl;
}

void Gomoku::AI::addToSearchBoard(uint8_t x, uint8_t y, uint8_t color)
{
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

void Gomoku::AI::removeFromSearchBoard(uint8_t x, uint8_t y)
{
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

void Gomoku::AI::displaySearchBoard()
{
    for (int i = 0; i < 20; ++i) {
        std::cout << "DEBUG | ";
        if (i == 0) {
            std::cout << "    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19" << std::endl;
            std::cout << "DEBUG | ";
        }
        for (int j = 0; j < 20; ++j) {
            if (j == 0) {
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

int Gomoku::AI::evaluateBoard(bool debug) {
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

Gomoku::Position Gomoku::AI::computeFirstEvaluation(Board &board, Board &searchBoard)
{
    Position bestMove(21, 21);

    for (uint8_t x = 0; x < 20; ++x) {
        for (uint8_t y = 0; y < 20; ++y) {
            if (searchBoard.board[x][y] == Gomoku::Color::TO_EXPLORE) {
                Gomoku::Position move = Gomoku::Position(x, y);
                addToSearchBoard(move.x, move.y, (uint8_t)Gomoku::Color::AI);
                board.playMove(move, Gomoku::Color::AI);

                int score = evaluateBoard();

                removeFromSearchBoard(move.x, move.y);
                board.undoMove(move);
                if (score == std::numeric_limits<int>::max() ||
                    score == std::numeric_limits<int>::min()) {
                    bestMove = move;
                    std::cout << "DEBUG Found a winning or blocking move" << std::endl;
                    return bestMove;
                }
            }
        }
    }
    return bestMove;
}

bool Gomoku::AI::checkScore(int &bestScore, Position &bestMove, int &score, Position &move)
{
    if (score == bestScore) {
        if (rand() % 2 == 0) {
            bestScore = score;
            bestMove = move;
        }
    } else if (score > bestScore) {
        bestScore = score;
        bestMove = move;
        if (bestScore >= 10000000)
            return true;
    }
    return false;
}

void Gomoku::AI::displayExecutionTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
    std::chrono::time_point<std::chrono::high_resolution_clock> end, int &maxDepth)
{
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    int seconds = duration / 1'000'000;
    int milliseconds = (duration % 1'000'000) / 1'000;
    int microseconds = duration % 1'000;

    std::cout << "DEBUG Execution time for getBestMove : " << seconds << "s "
    << milliseconds << "ms "
    << microseconds << "µs" << std::endl;

    if ((seconds > 0 && maxDepth > 0) || (milliseconds > 200 && maxDepth > 0)) {
        std::cout << "DEBUG Reducing depth" << std::endl;
        maxDepth--;
    }
}

Gomoku::Position Gomoku::AI::getBestMove()
{
    Position bestMove;
    Position firstMove = computeFirstEvaluation(board, searchBoard);
    int bestScore = std::numeric_limits<int>::min();
    int depth = maxDepth;
    auto getBestMoveStart = std::chrono::high_resolution_clock::now();

    if (firstMove.x != 21 && firstMove.y != 21)
        return firstMove;
    for (uint8_t x = 0; x < 20; ++x) {
        for (uint8_t y = 0; y < 20; ++y) {
            if (searchBoard.board[x][y] == Color::TO_EXPLORE) {
                Position move = Position(x, y);
                addToSearchBoard(move.x, move.y, (uint8_t)Color::AI);
                board.playMove(move, Color::AI);
                int score = principalVariationSearch(searchBoard, depth, false,
                    std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                removeFromSearchBoard(move.x, move.y);
                board.undoMove(move);
                if (checkScore(bestScore, bestMove, score, move))
                    return bestMove;
            }
        }
        if (bestScore >= 10000000)
            break;
    }
    auto getBestMoveEnd = std::chrono::high_resolution_clock::now();
    displayExecutionTime(getBestMoveStart, getBestMoveEnd, maxDepth);
    std::cout << "DEBUG Best move found: " << (int)bestMove.x << ","
        << (int)bestMove.y << " with score: " << bestScore << " using depth: "
        << maxDepth + 1 << std::endl;
    if (bestScore == std::numeric_limits<int>::min())
        bestMove = Position(10, 10);
    return bestMove;
}

int Gomoku::AI::principalVariationSearch(Board &exploratingBoard, uint8_t depth,
    bool isMaximizing, int alpha, int beta)
{
    uint64_t zobristKey = tt.computeZobristHash(exploratingBoard);
    auto it = tt.transpositionTable.find(zobristKey);
    int score = 0;

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
    score = evaluateBoard();
    if (depth <= 0 || score >= 10000000)
        return score;
    if (isMaximizing) {
        return doMax(exploratingBoard, zobristKey, depth, alpha, beta);
    } else {
        return doMin(exploratingBoard, zobristKey, depth, alpha, beta);
    }
}

int Gomoku::AI::doMax(Board &exploratingBoard, uint64_t &zobristKey,
    uint8_t depth, int alpha, int beta)
{
    bool firstChild = true;
    std::vector<Position> moves = generateMoves(exploratingBoard);

    for (const auto &move : moves) {
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
}

int Gomoku::AI::doMin(Board &exploratingBoard, uint64_t &zobristKey,
    uint8_t depth, int alpha, int beta)
{
    bool firstChild = true;
    std::vector<Position> moves = generateMoves(exploratingBoard);

    for (const auto &move : moves) {
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

std::vector<Gomoku::Position> Gomoku::AI::generateMoves(Board &boardTmp)
{
    std::vector<Position> moves;

    for (uint8_t x = 0; x < 20; ++x) {
        for (uint8_t y = 0; y < 20; ++y) {
            if (boardTmp.board[x][y] == Color::TO_EXPLORE)
                moves.push_back(Position(x, y));
        }
    }
    return moves;
}

void Gomoku::AI::stockIntoTranspositionTable(uint64_t &zobristKey, uint8_t &depth,
    int &bestValue, int &alpha, int &beta)
{
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
