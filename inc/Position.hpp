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

            uint8_t x;
            uint8_t y;
        protected:
        private:
    };
};

#endif // POSITION_HPP_
