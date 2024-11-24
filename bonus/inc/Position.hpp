/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Position Class
*/

#ifndef POSITION_HPP_
    #define POSITION_HPP_

    #include <cstdint>

namespace Gomoku {
    /**
     * @brief Class for the position
     * 
     * This class contains the x and y position of a stone in our board
     */
    class Position {
        public:

            Position() = default;
            Position(uint8_t posX, uint8_t posY) : x(posX), y(posY) {};
            ~Position() = default;

            bool operator==(const Position &other) const {
                return x == other.x && y == other.y;
            }

            uint8_t x;
            uint8_t y;

        protected:
        private:
    };

    /**
     * @brief Hash function for Position
     * 
     * Provides a way to hash a Position for use in unordered containers.
     */
    struct PositionHash {
        std::size_t operator()(const Position &pos) const noexcept {
            // Combine the hash of x and y using XOR and bit-shifting
            std::size_t xHash = std::hash<int>()(pos.x);
            std::size_t yHash = std::hash<int>()(pos.y);
            return xHash ^ (yHash << 1);
        }
    };

};

namespace std {
    /**
     * @brief std::hash specialization for Gomoku::Position
     */
    template <>
    struct hash<Gomoku::Position> {
        std::size_t operator()(const Gomoku::Position &pos) const noexcept {
            return Gomoku::PositionHash()(pos);
        }
    };
}

#endif // POSITION_HPP_
