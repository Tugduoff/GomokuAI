/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Stone Class
*/

#ifndef STONE_HPP_
    #define STONE_HPP_

    #include "Position.hpp"

/**
 * @brief Namespace for the Gomoku game
 * 
 * This namespace contains all the classes and functions for the Gomoku game
 */
namespace Gomoku {
    /**
     * @brief Enum for the color of the stone
     */
    enum class Color : uint8_t {
        BLACK,
        WHITE,
        NONE
    };

    /**
     * @brief Class for the stone
     * 
     * This class contains the position and color of the stone
     */
    class Stone {
        public:

            Stone() = default;
            Stone(Position stonePos, Color stoneColor) : pos(stonePos), color(stoneColor) {};
            ~Stone() = default;

            Position pos;
            Color color;

        protected:
        private:
    };
};

#endif // STONE_HPP_
