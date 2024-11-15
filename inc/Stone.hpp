/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Stone Class
*/

#ifndef STONE_HPP_
    #define STONE_HPP_

    #include <iostream>
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
        EMPTY = 0,
        AI = 1,
        ENEMY = 2,
        TO_EXPLORE = 3,
        OUT_OF_BOUND = 4,
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

    /**
     * @brief Overload of the << operator for the Color enum
     * 
     * This function is used to display the color of the stone
     * 
     * @param os The output stream
     * @param color The color to display
     * @return std::ostream &The output stream
     */
    std::ostream &operator<<(std::ostream &os, const Color &color);
};

#endif // STONE_HPP_
