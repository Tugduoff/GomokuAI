/*
** EPITECH PROJECT, 2024
** GomokuAI
** File description:
** Main
*/

#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "CommandHandler.hpp"

std::mutex mtx;
std::condition_variable cv;
std::condition_variable cv_done;
std::string cmd;
bool ready = false;


void readCmd() {
    while (true) {
        std::string tempCmd;
        std::cin.clear();
        std::getline(std::cin, tempCmd);
        std::unique_lock<std::mutex> lock(mtx);
        cmd = tempCmd;
        ready = true;
        std::cout << "DEBUG Read command: " << tempCmd << std::endl;
        cv.notify_one();
    }
}

void executeCmd(Gomoku::CommandHandler &handler) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return ready; });
        std::cout << "DEBUG Executing command: " << cmd << std::endl;
        if (cmd.empty()) {
            ready = false;
            continue;
        }
        handler.execute(cmd);
        ready = false;
    }
}

int main(void) {
    Gomoku::AI ai;
    Gomoku::CommandHandler handler(ai);
    std::string cmd;

    std::thread reader(readCmd);
    std::thread executor(executeCmd, std::ref(handler));

    srand(time(NULL));

    reader.join();
    executor.join();
    return 0;
}
