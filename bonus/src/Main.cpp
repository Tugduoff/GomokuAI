/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Main
*/

#include <string>
#include "CommandHandler.hpp"

int main(int ac, char **av) {
    std::cout << "Parameters: "<< std::atoi(av[1]) << " " << std::atoi(av[2]) << " " << std::atoi(av[3]) << " " << std::atoi(av[4]) << " " << std::atoi(av[5]) << " " << std::atoi(av[6]) << " " << std::atoi(av[7]) << " " << std::atoi(av[8]) << " " << std::atoi(av[9]) << " " << std::atoi(av[10]) << " " << std::atoi(av[11]) << " " << std::atoi(av[12]) << " " << std::atoi(av[13]) << " " << std::atoi(av[14]) << std::endl;
    Gomoku::AI ai(std::atoi(av[1]), std::atoi(av[2]), std::atoi(av[3]), std::atoi(av[4]),
        std::atoi(av[5]), std::atoi(av[6]), std::atoi(av[7]), std::atoi(av[8]), std::atoi(av[9]),
        std::atoi(av[10]), std::atoi(av[11]), std::atoi(av[12]), std::atoi(av[13]), std::atoi(av[14]));
    Gomoku::CommandHandler handler(ai);
    std::string cmd;

    srand(time(NULL));

    while (true) {
        std::cin.clear();
        cmd.clear();
        if (!(std::cin >> cmd)) {
            std::cout << "DEBUG End of input detected. Exiting" << std::endl;
            break;
        }
        handler.execute(cmd);
    }
    return 0;
}
