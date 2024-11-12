/*
** EPITECH PROJECT: Gomoku, 2024
** File description:
** Command handler class
*/

#ifndef COMMAND_HANDLER_HPP
    #define COMMAND_HANDLER_HPP

    #include <sstream>
    #include <functional>
    #include <vector>
    #include <string>
    #include "AI.hpp"

namespace Gomoku {
    /**
     * @brief Class for handling commands
     *
     * This class contains the command handler for the AI
     */
    class CommandHandler {
        public:
            CommandHandler(Gomoku::AI &ai) : __ai(ai), __commands({
                {"START", [this](std::string cmdArgs) { handleStart(cmdArgs); }},
                {"TURN", [this](std::string cmdArgs) { handleTurn(cmdArgs); }},
                {"BEGIN", [this](std::string cmdArgs) { handleBegin(cmdArgs); }},
                {"BOARD", [this](std::string cmdArgs) { handleBoard(cmdArgs); }},
                {"INFO", [this](std::string cmdArgs) { handleInfo(cmdArgs); }},
                {"ABOUT", [this](std::string cmdArgs) { handleAbout(cmdArgs); }},
                {"END", [this](std::string cmdArgs) { handleEnd(cmdArgs); }}
            }) {}

            void execute(const std::string &cmd) {
                std::vector<std::string> args = split(cmd, ' ');
                std::string command = args[0];
                std::string cmdArgs = args.size() > 1 ? args[1] : "";

                auto it = __commands.find(command);

                if (it != __commands.end()) {
                    it->second(cmdArgs);
                } else {
                    handleUnknown(command);
                }
            }

            /**
             * @brief Split a string
             *
             * This function splits a string by a delimiter
             *
             * @param str The string to split
             * @param delimiter The delimiter
             * @return std::vector<std::string> The splitted string
             */
            std::vector<std::string> split(const std::string &str, char delimiter) {
                std::vector<std::string> tokens;
                std::string token;
                std::stringstream ss(str);

                while (std::getline(ss, token, delimiter)) {
                    tokens.push_back(token);
                }
                return tokens;
            }

        private:

            Gomoku::AI &__ai;
             std::unordered_map<std::string, std::function<void(std::string)>> __commands;

            /**
             * @brief Handle the START command
             * 
             * This function is called when the START command is received
             * 
             * Example: \n
             * START 20
             */
            void handleStart(std::string cmdArgs) {
                int size;
                std::istringstream iss(cmdArgs);

                __ai.board = Gomoku::Board();
                std::cout << "OK" << std::endl;
            }

            /**
             * @brief Handle the TURN command
             * 
             * This function is called when the TURN command is received
             * 
             * Example: \n
             * TURN 10,10
             * 
             * @note The AI will note the enemy move and play its turn
             */
            void handleTurn(std::string cmdArgs) {
                std::string args;
                std::getline(std::cin >> std::ws, args);

                int x, y;
                char comma;
                std::istringstream iss(args);

                if (!(iss >> x >> comma >> y) || comma != ',') {
                    std::cerr << "ERROR Invalid TURN command format. Expected 'TURN X,Y'" << std::endl;
                    return;
                }

                std::cout << "DEBUG Enemy played at " << x << "," << y << std::endl;
                __ai.board.board[(uint8_t)x][(uint8_t)y] = 2;
                __ai.turn();
            }

            /**
             * @brief Handle the BEGIN command
             * 
             * This function is called when the BEGIN command is received
             * 
             * Example: \n
             * BEGIN
             */
            void handleBegin(std::string cmdArgs) {
                __ai.turn();
            }

            /**
             * @brief Handle the BOARD command
             * 
             * This function is called when the BOARD command is received
             * 
             * Example: \n
             * BOARD \n
             * 10,10,1 \n
             * 10,11,2 \n
             * 11,11,1 \n
             * 9,10,2 \n
             * DONE
             * 
             * @note The AI will note the board state and when DONE is received, play its turn
             * 
             * 1 = AI \n
             * 2 = Enemy \n
             * 0 = Empty \n
             * 3 = Win Position
             */
            void handleBoard(std::string cmdArgs) {
                __ai.board = Gomoku::Board();

                std::string line;
                while (std::getline(std::cin, line)) {
                    if (line == "DONE") break;

                    std::istringstream iss(line);
                    int x, y, color;
                    if (!(iss >> x >> y >> color)) {
                        std::cerr << "ERROR Invalid board data format." << std::endl;
                        continue;
                    }
                    __ai.board.board[x][y] = color;
                }
                __ai.turn();
            }

            /**
             * @brief Handle the INFO command
             * 
             * This function is called when the INFO command is received
             * 
             * Example: \n
             * INFO folder /tmp \n
             * INFO timeout_match 1000 \n
             * INFO timeout_turn 100 \n
             * INFO max_memory 10000
             */
            void handleInfo(std::string cmdArgs) {
                std::string key;
                std::cin >> key;

                if (key == "folder") {
                    std::string folder;
                    std::cin >> folder;
                } else if (key == "evaluate") {
                    int x, y;
                    std::cin >> x >> y;
                } else {
                    long value;
                    std::cin >> value;
                }
            }

            /**
             * @brief Handle the ABOUT command
             * 
             * This function is called when the ABOUT command is received
             * 
             * Example: \n
             * ABOUT
             */
            void handleAbout(std::string cmdArgs) {
                std::cout << "name=\"Centurion\", version=\"1.1\", author=\"Tugdu & Traz\", country=\"France\"" << std::endl;
            }

            /**
             * @brief Handle the END command
             * 
             * This function is called when the END command is received
             * 
             * Example: \n
             * END
             */
            void handleEnd(std::string cmdArgs) {
                exit(0);
            }

            /**
             * @brief Handle unknown command
             * 
             * This function is called when an unknown command is received
             * 
             * @param cmd The unknown command
             */
            void handleUnknown(const std::string &cmd) {
                std::cerr << "ERROR Unknown command \"" << cmd << "\"" << std::endl;
            }
    };
};

#endif // COMMAND_HANDLER_HPP
