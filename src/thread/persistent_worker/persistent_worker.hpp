#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>

#define PAUSE_BT_TASK 0 // ms

/**
 * @brief Persistent worker thread that executes tasks in a continuous loop.
 *
 * This class provides a background worker thread that continuously executes registered
 * tasks in a loop. Tasks can be dynamically added and removed during runtime, making
 * it ideal for background processing, periodic operations, or event-driven task execution.
 * The worker uses condition variables for efficient thread synchronization and CPU usage.
 *
 * @note Allows dynamic addition/removal of tasks during runtime
 * @note Supports configurable pause between each task execution (PAUSE_BT_TASK in milliseconds)
 * @details Thread-safe task management using mutex and condition variables
 * @details Automatically starts worker thread on construction and stops on destruction
 * @details Worker thread sleeps when no tasks are available, reducing CPU usage
 *
 * @code
 * // Create worker (automatically starts background thread)
 * PersistentWorker worker;
 *
 * // Add tasks that will run continuously
 * worker.addTask("logger", []() {
 *     std::cout << "Log entry at " << getCurrentTime() << std::endl;
 * });
 *
 * worker.addTask("cleanup", []() {
 *     cleanupTempFiles();
 * });
 *
 * worker.addTask("heartbeat", []() {
 *     sendHeartbeat();
 * });
 *
 * // Tasks now run continuously in background...
 * // Do other work...
 *
 * // Remove specific task
 * worker.removeTask("cleanup");
 *
 * // Worker stops automatically when destroyed
 * @endcode
 *
 * @warning Tasks should be lightweight to avoid blocking other tasks in the loop
 * @note Task execution order is not guaranteed due to unordered_map usage
 */
class PersistentWorker
{
private:
    std::mutex                                             _mtx;
    std::condition_variable                                _cv;
    std::unordered_map<std::string, std::function<void()>> _tasks;
    bool                                                   _running = true;
    std::thread                                            _thread;

    void loop();

public:
    PersistentWorker();
    ~PersistentWorker();

    void addTask(const std::string& name, const std::function<void()>& jobToExecute);

    void removeTask(const std::string& name);
};
#endif