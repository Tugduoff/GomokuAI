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
            Board() {
                for (uint8_t i = 0; i < 20; i++) {
                    for (uint8_t j = 0; j < 20; j++) {
                        board[i][j] = 3; // Default value for empty cell
                    }
                }
            };
            ~Board() = default;

            uint8_t board[20][20];
        protected:
        private:
    };
};

#endif // BOARD_HPP_
