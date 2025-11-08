#ifndef THREAD_HPP
#define THREAD_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <thread>

#include "../thread_safe_iostream/thread_safe_iostream.hpp"

/**
 * @brief Simple Thread Wrapper
 *
 * This class provides a simple wrapper around std::thread to manage a thread
 * that executes a user-defined function. It includes basic start and stop
 * functionality along with a thread-safe output stream for logging.
 *
 * @note Uses ThreadSafeIOStream for thread-safe logging
 *
 * @code
 * Thread myThread("MyThread", []() {
 *     // Thread work here
 *     for (int i = 0; i < 10; ++i) {
 *         std::cout << "Thread iteration " << i << std::endl;
 *     }
 * });
 *
 * myThread.start();
 *
 * // Do other work...
 *
 * myThread.stop();
 * @endcode
 */
class Thread
{
private:
    std::function<void()> _funct;
    std::thread           _t;
    ThreadSafeIOStream    _stream;

public:
    Thread(const std::string& name, std::function<void()> funcToExecute);
    ~Thread();

    void start();
    void stop();
};

#endif