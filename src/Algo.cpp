/*
** EPITECH PROJECT, 2024
** Gomoku
** File description:
** Algo
*/

#include "Algo.hpp"

Gomoku::Position Gomoku::Algo::computeFirstEvaluation(Board &board, Board &searchBoard)
{
    Position bestMove(21, 21);

    for (uint8_t x = 0; x < 20; ++x) {
        for (uint8_t y = 0; y < 20; ++y) {
            if (searchBoard.board[x][y] == Color::TO_EXPLORE) {
                Position move = Position(x, y);
                __ai.addToSearchBoard(move.x, move.y, (uint8_t)Color::AI);
                board.playMove(move, Color::AI);

                int score = __ai.evaluateBoard();

                __ai.removeFromSearchBoard(move.x, move.y);
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

bool Gomoku::Algo::checkScore(int &bestScore, Position &bestMove, int &score, Position &move)
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

void Gomoku::Algo::displayExecutionTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
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

Gomoku::Position Gomoku::Algo::getBestMove()
{
    Position bestMove;
    Position firstMove = computeFirstEvaluation(__ai.board, __ai.searchBoard);
    int bestScore = std::numeric_limits<int>::min();
    int depth = __ai.maxDepth;
    auto getBestMoveStart = std::chrono::high_resolution_clock::now();
    std::mutex mtx;
    std::vector<std::thread> threads;
    std::multimap<int, Position, std::greater<int>> bestMoves;
    std::vector<std::future<std::pair<int, Position>>> futures;
    int8_t activeThreads = 0;
    int8_t numThreads = std::thread::hardware_concurrency();

    if (numThreads == 0)
        numThreads = 1;
    if (firstMove.x != 21 && firstMove.y != 21)
        return firstMove;
    for (uint8_t x = 0; x < 20; ++x) {
        for (uint8_t y = 0; y < 20; ++y) {
            if (__ai.searchBoard.board[x][y] == Color::TO_EXPLORE) {
                Position move = Position(x, y);
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    __ai.addToSearchBoard(move.x, move.y, (uint8_t)Color::AI);
                    __ai.board.playMove(move, Color::AI);
                }
                if (activeThreads >= numThreads) {
                    for (auto& thread : threads)
                        thread.join();
                    threads.clear();
                    activeThreads = 0;
                }
                std::promise<std::pair<int, Position>> resultPromise;
                futures.push_back(resultPromise.get_future());
                threads.emplace_back([this](Board &board, uint8_t searchDepth, bool flag, int alpha, int beta, std::mutex &mutex, std::promise<std::pair<int, Position>> resultPromise2, Position movec) {
                    try {
                        int score = this->principalVariationSearch(board, searchDepth, flag, alpha, beta, mutex);
                        resultPromise2.set_value(std::make_pair(score, movec));
                    } catch (...) {
                        resultPromise2.set_exception(std::current_exception());
                    }
                }, std::ref(__ai.board), depth, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), std::ref(mtx), std::move(resultPromise), move);
                activeThreads++;
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    __ai.removeFromSearchBoard(move.x, move.y);
                    __ai.board.undoMove(move);
                }
                //if (checkScore(bestScore, bestMove, score, move))
                //    return bestMove;
                //}
            }
        }
    }
    for (auto& thread : threads)
        thread.join();
    for (auto& future : futures) {
        std::pair<int, Position> threadResult = future.get();
        std::cout << "DEBUG Best move found: " << (int)threadResult.second.x << "," << (int)threadResult.second.y << " with score: " << bestScore << std::endl;
        if (threadResult.first == bestScore) {
            if (rand() % 2 == 0) {
                bestScore = threadResult.first;
                bestMoves.insert(std::make_pair(threadResult.first, threadResult.second));
            }
        } else if (threadResult.first > bestScore) {
            bestScore = threadResult.first;
            bestMoves.insert(std::make_pair(threadResult.first, threadResult.second));
        }
        if (bestScore >= 10000000) {
            break;
        }
    }
    auto getBestMoveEnd = std::chrono::high_resolution_clock::now();
    displayExecutionTime(getBestMoveStart, getBestMoveEnd, __ai.maxDepth);
    std::cout << "DEBUG Best move found: " << (int)bestMove.x << ","
        << (int)bestMove.y << " with score: " << bestScore << " using depth: "
        << __ai.maxDepth + 1 << std::endl;
    bestMove = bestMoves.begin()->second;
    if (bestScore == std::numeric_limits<int>::min())
        bestMove = Position(10, 10);
    return bestMove;
}

int Gomoku::Algo::principalVariationSearch(Board &exploratingBoard, uint8_t depth,
    bool isMaximizing, int alpha, int beta, std::mutex &mtx)
{
    uint64_t zobristKey;
    std::unordered_map<uint64_t, TranspositionTable>::iterator it;
    {
        std::lock_guard<std::mutex> lock(mtx);
        zobristKey = __ai.tt.computeZobristHash(exploratingBoard);
        it = __ai.tt.transpositionTable.find(zobristKey);
    }
    int score = 0;

    if (it != __ai.tt.transpositionTable.end()) {
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
    score = __ai.evaluateBoard();
    if (depth <= 0 || score >= 10000000)
        return score;
    if (isMaximizing) {
        return doMax(exploratingBoard, zobristKey, depth, alpha, beta, mtx);
    } else {
        return doMin(exploratingBoard, zobristKey, depth, alpha, beta, mtx);
    }
}

int Gomoku::Algo::doMax(Board &exploratingBoard, uint64_t &zobristKey,
    uint8_t depth, int alpha, int beta, std::mutex &mtx)
{
    bool firstChild = true;
    std::vector<Position> moves = generateMoves(exploratingBoard);

    for (const auto &move : moves) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            __ai.addToSearchBoard(move.x, move.y, 2);
            __ai.board.playMove(move, Color::ENEMY);
        }
        int res;
        if (firstChild) {
            res = principalVariationSearch(exploratingBoard, depth - 1, false, alpha, beta, mtx);
            firstChild = false;
        } else {
            res = principalVariationSearch(exploratingBoard, depth - 1, false, alpha, alpha + 1, mtx);
            if (res > alpha && res < beta) {
                res = principalVariationSearch(exploratingBoard, depth - 1, false, alpha, beta, mtx);
            }
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            __ai.removeFromSearchBoard(move.x, move.y);
            __ai.board.undoMove(move);
        }
        alpha = std::max(alpha, res);
    }
    stockIntoTranspositionTable(zobristKey, depth, alpha, alpha, beta);
    return alpha;
}

int Gomoku::Algo::doMin(Board &exploratingBoard, uint64_t &zobristKey,
    uint8_t depth, int alpha, int beta, std::mutex &mtx)
{
    bool firstChild = true;
    std::vector<Position> moves = generateMoves(exploratingBoard);

    for (const auto &move : moves) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            __ai.addToSearchBoard(move.x, move.y, 1);
            __ai.board.playMove(move, Color::AI);
        }
        int res;
        if (firstChild) {
            res = principalVariationSearch(exploratingBoard, depth - 1, true, alpha, beta, mtx);
            firstChild = false;
        } else {
            res = principalVariationSearch(exploratingBoard, depth - 1, true, beta - 1, beta, mtx);
            if (res > alpha && res < beta) {
                res = principalVariationSearch(exploratingBoard, depth - 1, true, alpha, beta, mtx);
            }
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            __ai.removeFromSearchBoard(move.x, move.y);
            __ai.board.undoMove(move);
        }
        beta = std::min(beta, res);
    }
    stockIntoTranspositionTable(zobristKey, depth, beta, alpha, beta);
    return beta;
}

std::vector<Gomoku::Position> Gomoku::Algo::generateMoves(Board &boardTmp)
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

void Gomoku::Algo::stockIntoTranspositionTable(uint64_t &zobristKey, uint8_t &depth,
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
    __ai.tt.transpositionTable[zobristKey] = entry;
}
