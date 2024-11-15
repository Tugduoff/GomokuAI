/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** TranspositionTable
*/

#pragma once

#include <iostream>
#include <unordered_map>
#include "Position.hpp"

namespace Gomoku {
    /**
     * @brief Class for the transposition table
     * 
     * This class contains the transposition table
     */
    class TranspositionTable {
        public:
            TranspositionTable() = default;
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

            std::unordered_map<uint64_t, TranspositionTable> transpositionTable;
            Position position;
            int value;
            uint8_t depth;
            uint8_t flag;

        protected:
        private:
    };
};
