/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** TranspositionTable
*/

#pragma once

#include <iostream>
#include <random>
#include <unordered_map>
#include "Board.hpp"
#include "Stone.hpp"
#include "Position.hpp"

namespace Gomoku {
    /**
     * @brief Class for the transposition table
     * 
     * This class contains the transposition table
     */
    class TranspositionTable {
        public:
            TranspositionTable() {};
            TranspositionTable(Position pos, uint8_t val, uint8_t dep, uint8_t fl)
                : position(pos), value(val), depth(dep), flag(fl) {};
            ~TranspositionTable() = default;

            bool operator==(const TranspositionTable &other) const {
                return position == other.position && value == other.value && depth == other.depth && flag == other.flag;
            }

            TranspositionTable &operator=(const TranspositionTable &other) {
                position = other.position;
                value = other.value;
                depth = other.depth;
                flag = other.flag;
                return *this;
            }

            void initializeZobristTable() {
                std::mt19937_64 rng(12345);
                std::uniform_int_distribution<uint64_t> dist;

                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 20; j++) {
                        for (int k = 0; k < 2; k++) {
                            zobristTable[i][j][k] = dist(rng);
                        }
                    }
                }
            }

            uint64_t computeZobristHash(Board &exploratingBoard) {
                uint64_t hash = 0;

                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 20; j++) {
                        if (exploratingBoard.board[i][j] != Color::EMPTY) {
                            int player = (exploratingBoard.board[i][j] == Color::AI) ? 1 : 2;
                            hash ^= zobristTable[i][j][player];
                        }
                    }
                }
                return hash;
            }

            std::unordered_map<uint64_t, TranspositionTable> transpositionTable;
            uint8_t zobristTable[20][20][2];
            Position position;
            int value;
            uint8_t depth;
            int8_t flag;

        protected:
        private:
    };
};
