/*
** EPITECH PROJECT, 2024
** Gomoku
** File description:
** Threads
*/

#ifndef THREADS_HPP_
    #define THREADS_HPP_

    #include <thread>
    #include <future>

namespace Gomoku
{
    /**
     * @brief The Threads class is responsible for managing the threads
     * in the Gomoku.
     * 
     * @tparam Function The Function template parameter is the function
     * to be executed in the thread.
     * @tparam Args The Args template parameter is the arguments
     * to be passed to the function.
     * @tparam ReturnVal The ReturnVal template parameter is the return value
     * of the function.
     */
    template <typename ReturnVal>
    class Threads {
        public:

            /**
             * @brief Construct a new Threads object
             * 
             * @param function Function to be executed in the thread.
             * @param args Arguments to be passed to the function.
             * @param returnVal Return value of the function.
             */
            template <typename Function, typename... Args>
            Threads(Function&& function, Args&&... args)
                : _promise(),
                  _future(_promise.get_future())
            {
                _thread = std::thread(function, std::ref(_promise), args...);
            }

            /**
             * @brief Destroy the Threads object
             * 
             */
            ~Threads()
            {
                if (_thread.joinable()) {
                    _thread.join();
                }
            }

            // Methods
            /**
             * @brief Joins the thread. Wait for the threads to finish
             * 
             */
            void join()
            {
                if (_thread.joinable()) {
                    _thread.join();
                }
            }

            // Getters

            /**
             * @brief Get the thread object
             * 
             * @return std::thread& 
             */
            std::thread &getThread() { return _thread; }

            /**
             * @brief Get the future object => Get the return value of the function
             * passed to the thread.
             * 
             * @return std::future<ReturnVal> 
             */
            std::future<ReturnVal> &getFuture() { return _future; }

        protected:
        private:
            std::thread _thread;
            std::promise<ReturnVal> _promise;
            std::future<ReturnVal> _future;
    };
}

#endif /* !THREADS_HPP_ */
