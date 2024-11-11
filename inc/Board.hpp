/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Board Class
*/

#ifndef BOARD_HPP_
    #define BOARD_HPP_

    #include <cstdint>

namespace Gomoku {
    /**
     * @brief Class for the board
     * 
     * This class contains the board of the game
     */
    class Board {
        public:
            Board() = default;
            ~Board() = default;

            uint8_t board[20][20] = {0};
        protected:
        private:
    };
};

#endif // BOARD_HPP_
