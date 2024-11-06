/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Commands
*/

#ifndef COMMANDS_HPP
    #define COMMANDS_HPP

    #include <iostream>
    #include <map>
    #include <string>
    #include <array>

    class Commands {
        public:
            /**
             * @brief Construct a new Commands object
             *
             */
            Commands()
            {
            }

            /**
             * @brief Destroy the Commands object
             *
             */
            ~Commands();

            /**
             * @brief Return the command from a line
             *
             * @param std::string line
             * @return std::string
             */
            std::string getCommand(std::string line)
            {
                std::string command = line.substr(0, line.find(" "));
                return command;
            }

            /**
             * @brief Awnser to the start command
             *
             * @param std::map<std::string, int8_t> infoDict
             */
            void startCommand(void)
            {
                std::cout << "OK" << std::endl;
            }

            /**
             * @brief Awnser to the begin command
             *
             * @param std::map<std::string, int8_t> infoDict
             */
            void turnCommand(int8_t x = 1, int8_t y = 1)
            {
                std::cout << x << "," << y << std::endl;
            }

            /**
             * @brief Awnser to the board command
             *
             * @param std::map<std::string, int8_t> infoDict
             */
            void boardCommand(int8_t x = 1, int8_t y = 1)
            {
                turnCommand(x, y);
            }

            /**
             * @brief Awnser to the info command
             *
             * @param std::map<std::string, int8_t> infoDict
             */
            void infoCommand(std::string)
            {
                return;
            }

            void aboutCommand(void)
            {
                std::cout << "name=\"Centurion\", version=\"1.0\", author=\"Tugdu & Traz\", country=\"France\"" << std::endl;
            }


        protected:
        private:
    };

#endif
