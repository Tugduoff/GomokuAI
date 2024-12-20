/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Line Class
*/

#ifndef LINE_HPP_
    #define LINE_HPP_

    #include "Stone.hpp"

/**
 * @brief Namespace for the Gomoku game
 * 
 * This namespace contains all the classes and functions for the Gomoku game
 */
namespace Gomoku {

    /**
     * @brief Class for the stone
     * 
     * This class contains the position and color of the stone
     */
    class Line {
        public:

            Line() = default;
            Line(int lineScore, int8_t lineDx, int8_t lineDy, Color lineColor, std::array<Stone, 9> &&linePositions) :
                score(lineScore), dx(lineDx), dy(lineDy), color(lineColor), positions(linePositions) {};
            ~Line() = default;

            // Operator ==
            bool operator==(const Line &line) const {
                bool samePositions = false;
                Stone center = positions[4]; // If center stone is contained in the line, the line is the same
                for (const auto &stone : line.positions) {
                    if (stone.pos == center.pos) {
                        samePositions = true;
                        break;
                    }
                }
                return (line.score == score && line.dx == dx && line.dy == dy && line.color == color && samePositions);
            }

            int score;
            int8_t dx;
            int8_t dy;
            Color color;
            std::array<Stone, 9> positions;
            bool dirty = false;

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
    std::ostream &operator<<(std::ostream &os, const Line &line);
};

#endif // LINE_HPP_
