/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** AI
*/

#include "AI.hpp"

void Gomoku::AI::turn()
{
    static int nbrTurn = 0;
    int score = evaluateBoard();
    //int tmp = checkGameStatus();
    //if (tmp != 0) {
    //    std::cout << "DEBUG Status: " << tmp << std::endl;
    //    if (tmp == -1) {
    //        std::cout << "DEBUG LOSE" << std::endl;
    //    } else {
    //        std::cout << "DEBUG WIN" << std::endl;
    //    }
    //    return;
    //}
    std::cout << "DEBUG Status: " << score << std::endl;
    if (score == std::numeric_limits<int>::max()) {
        std::cout << "DEBUG WIN" << std::endl;
        exit(nbrTurn);
    } else if (score == std::numeric_limits<int>::min()) {
        std::cout << "DEBUG LOSE" << std::endl;
        exit(401);
    }

    std::cout << "DEBUG Max depth: " << maxDepth << std::endl;

    Position bestMove = getBestMove();
    uint8_t x = bestMove.x;
    uint8_t y = bestMove.y;

    board.playMove(bestMove, Color::AI);

    score = evaluateBoard();
    displayBoard(bestMove, Color::AI);

    std::cout << "DEBUG Player played at " << (int)x << "," << (int)y << std::endl;
    std::cout << "DEBUG Status: " << score << std::endl;
    std::cout << "DEBUG Max depth: " << maxDepth << std::endl;
    if (score == std::numeric_limits<int>::max()) {
        std::cout << "DEBUG WIN" << std::endl;
        displayBoard(bestMove, Color::AI);
        exit(nbrTurn);
    } else if (score == std::numeric_limits<int>::min()) {
        std::cout << "DEBUG LOSE" << std::endl;
        displayBoard(bestMove, Color::AI);
        exit(401);
    }
    std::cout << (int)x << "," << (int)y << std::endl;
    nbrTurn++;
}

void Gomoku::AI::displayBoard(const Position &pos, Color color)
{
    std::cout << "DEBUG | Max depth: " << maxDepth << std::endl;
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
            if (i == pos.x && j == pos.y)
                std::cout << "\033[92m" << (color == Color::AI ? 'X' : 'O') << "\033[0m  ";
            else
                std::cout << board.board[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}

int Gomoku::AI::checkGameStatus() {
    const int size = 20;
    static int nbrTurn = 0;
    const int winCondition = 5;

    nbrTurn++;
    std::cout << "DEBUG Count: " << nbrTurn << std::endl;
    auto checkLine = [&](int x, int y, int dx, int dy, Gomoku::Color color) -> bool {
        int count = 0;
        for (int i = 0; i < winCondition; ++i) {
            int nx = x + i * dx;
            int ny = y + i * dy;
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && board.board[nx][ny] == color) {
                count++;
            } else {
                break;
            }
        }
        return count == winCondition;
    };
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (board.board[x][y] == Gomoku::Color::ENEMY || board.board[x][y] == Gomoku::Color::AI) {
                Gomoku::Color currentColor = board.board[x][y];
                if (checkLine(x, y, 1, 0, currentColor) ||
                    checkLine(x, y, 0, 1, currentColor) ||
                    checkLine(x, y, 1, 1, currentColor) ||
                    checkLine(x, y, 1, -1, currentColor))
                {
                    int tmp = currentColor == Gomoku::Color::ENEMY ? 401 : nbrTurn;
                    exit(tmp);
                }
            }
        }
    }

    return 0;
}

int Gomoku::AI::evaluateBoard(bool debug) {
    int score = 0;

    if (debug) {
        // for (auto &lines : board.boardLines) {
        //     for (auto &line : lines) {
        //         std::cout << "DEBUG Line: " << line;
        //     }
        // }
        for (auto &line : board.uniqueLines) {
            std::cout << "DEBUG Unique Line: " << line;
        }
    }
    for (auto &line : board.uniqueLines) {
        if (line.score == std::numeric_limits<int>::max()) {
            board.status = Board::WIN;
            std::cout << "DEBUG Found a winning line" << std::endl;
            return line.score;
        }
        if (line.score == std::numeric_limits<int>::min()) {
            board.status = Board::LOSE;
            std::cout << "DEBUG Found a losing line" << std::endl;
            return line.score;
        }
        score += line.score;
    }

    return score;
}

Gomoku::Position Gomoku::AI::computeFirstEvaluation()
{
    Position bestMove(21, 21);
    Position blockingMove(21, 21);

    std::vector<Position> moves = generateMoves(board);

    for (auto &move : moves) {
        board.playMove(move, Color::AI);

        int score = evaluateBoard();
        if (score == std::numeric_limits<int>::max()) {
            bestMove = move;
            std::cout << "DEBUG Found a winning move" << std::endl;
            board.undoMove(move);
            return bestMove;
        }

        board.undoMove(move);

        board.playMove(move, Color::ENEMY);

        score = evaluateBoard();
        if (score == std::numeric_limits<int>::min()) {
            blockingMove = move;
            std::cout << "DEBUG Found a losing move" << std::endl;
        }

        board.undoMove(move);
    }

    std::cout << "DEBUG Best move: " << (int)bestMove.x << "," << (int)bestMove.y << std::endl;
    std::cout << "DEBUG Blocking move: " << (int)blockingMove.x << "," << (int)blockingMove.y << std::endl;

    if (blockingMove.x != 21 && blockingMove.y != 21 && bestMove.x == 21 && bestMove.y == 21)
        return blockingMove;
    return bestMove;
}

bool Gomoku::AI::checkScore(int &bestScore, Position &bestMove, int &score, const Position &move)
{
    if (score == bestScore) {
        if (rand() % 2 == 0) {
            bestScore = score;
            bestMove = move;
        }
    } else if (score > bestScore) {
        bestScore = score;
        bestMove = move;
    }
    return false;
}

void Gomoku::AI::displayExecutionTime()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    int seconds = duration / 1'000'000;
    int milliseconds = (duration % 1'000'000) / 1'000;
    int microseconds = duration % 1'000;

    std::cout << "DEBUG Execution time for getBestMove : " << seconds << "s "
    << milliseconds << "ms "
    << microseconds << "µs" << std::endl;

    if (seconds >= 3 && maxDepth > 0) {
        std::cout << "DEBUG Reducing depth" << std::endl;
        maxDepth--;
    }
}

Gomoku::Position Gomoku::AI::getBestMove()
{
    start = std::chrono::high_resolution_clock::now();

    if (board.count(Color::AI) == 0 && board.count(Color::ENEMY) == 0) {
        return Position(10, 10);
    }

    Position firstEvaluation = computeFirstEvaluation();

    if (firstEvaluation.x != 21 && firstEvaluation.y != 21) {
        return firstEvaluation;
    }

    Position bestMove;

    std::vector<Position> moves = generateMoves(board);
    int depth = maxDepth;
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    int maxEval = std::numeric_limits<int>::min();

    for (const auto &move : moves) {
        board.playMove(move, Color::AI);

        // std::cout << "DEBUG Playing move (AI): " << (int)move.x << "," << (int)move.y << std::endl;
        int eval = minimax(depth - 1, false, alpha, beta);

        board.undoMove(move);

        if (eval > maxEval) {
            maxEval = eval;
            bestMove = move;
        }

        beta = std::min(beta, eval);
        if (beta <= alpha || isTimeOver()) {
            // std::cout << "DEBUG Pruning: beta (" << beta << ") <= alpha (" << alpha << ")\n";
            break;
        }
    }

    // std::cout << "DEBUG Best score doMax (AI): " << maxEval << " beta: " << beta << " alpha: " << alpha << " on depth: " << (int)depth << std::endl;
    displayExecutionTime();
    std::cout << "DEBUG Best move found: " << (int)bestMove.x << ","
        << (int)bestMove.y << " with score: " << maxEval << " using depth: "
        << maxDepth << std::endl;

    return bestMove;
}

int Gomoku::AI::minimax(uint8_t depth, bool isMaximizing, int alpha, int beta)
{
    // uint64_t zobristKey = tt.computeZobristHash(exploratingBoard);
    // auto it = tt.transpositionTable.find(zobristKey);

    // if (it != tt.transpositionTable.end()) {
    //     std::cout << "DEBUG Found in transposition table" << std::endl;
    //     const TranspositionTable& entry = it->second;
    //     if (entry.depth >= depth) {
    //         if (entry.flag == 0) {
    //             return entry.value;
    //         } else if (entry.flag == -1 && entry.value <= alpha) {
    //             return entry.value;
    //         } else if (entry.flag == 1 && entry.value >= beta) {
    //             return entry.value;
    //         }
    //     }
    // }

    if (depth <= 0 || board.status == Board::WIN || board.status == Board::LOSE || isTimeOver()) {
        int score = evaluateBoard();
        // std::cout << "DEBUG Score: " << score << " on depth: " << (int)depth << std::endl;
        return score;
    }

    if (isMaximizing) {
        return doMax(depth, alpha, beta);
    } else {
        return doMin(depth, alpha, beta);
    }
}

int Gomoku::AI::doMax(uint8_t depth, int alpha, int beta)
{
    std::vector<Position> moves = generateMoves(board);
    int maxEval = std::numeric_limits<int>::min();

    for (const auto &move : moves) {
        board.playMove(move, Color::ENEMY);
        // std::cout << "DEBUG Playing move (ENEMY): " << (int)move.x << "," << (int)move.y << std::endl;
        int eval = minimax(depth - 1, false, alpha, beta);
        board.undoMove(move);
        maxEval = std::max(maxEval, eval);
        alpha = std::max(alpha, eval);
        if (beta <= alpha || isTimeOver()) {
            // std::cout << "DEBUG Pruning: beta (" << beta << ") <= alpha (" << alpha << ")\n";
            break;
        }
    }
    // stockIntoTranspositionTable(zobristKey, depth, alpha, alpha, beta);
    // std::cout << "DEBUG Best score doMax (AI): " << maxEval << " alpha: " << alpha << " beta: " << beta << " on depth: " << (int)depth << std::endl;
    return maxEval;
}

int Gomoku::AI::doMin(uint8_t depth, int alpha, int beta)
{
    std::vector<Position> moves = generateMoves(board);
    int minEval = std::numeric_limits<int>::max();

    for (const auto &move : moves) {
        board.playMove(move, Color::AI);
        // std::cout << "DEBUG Playing move (AI): " << (int)move.x << "," << (int)move.y << std::endl;
        int eval = minimax(depth - 1, true, alpha, beta);
        board.undoMove(move);
        minEval = std::min(minEval, eval);
        beta = std::min(beta, eval);
        if (beta <= alpha || isTimeOver()) {
            // std::cout << "DEBUG Pruning: beta (" << beta << ") <= alpha (" << alpha << ")\n";
            break;
        }
    }
    // stockIntoTranspositionTable(zobristKey, depth, beta, alpha, beta);
    // std::cout << "DEBUG Best score doMin (ENEMY): " << minEval << " beta: " << beta << " alpha: " << alpha << " on depth: " << (int)depth << std::endl;
    return minEval;
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
