/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Board Class
*/

#ifndef BOARD_HPP_
    #define BOARD_HPP_

    #include "Stone.hpp"

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

            Stone board[20][20];
        protected:
        private:
    };
};

#endif // BOARD_HPP_
