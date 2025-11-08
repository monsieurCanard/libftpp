#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
/**
 * @brief Worker Pool for Concurrent Job Execution
 *
 * This class provides a pool of worker threads that can execute jobs concurrently.
 * Jobs can be added to the pool, and worker threads will pick them up and execute them.
 * The pool manages thread synchronization and job distribution automatically.
 *
 * @note Uses condition variables for efficient thread synchronization
 * @note Supports adding jobs as std::function<void()> or via IJobs interface
 *
 * @code
 * // Create a worker pool with 4 threads
 * WorkerPool pool(4);
 *
 * // Add a simple job
 * pool.addJob([]() {
 *     std::cout << "Hello from a worker thread!" << std::endl;
 * });
 *
 * // Define a job via IJobs interface
 * class MyJob : public WorkerPool::IJobs {
 * public:
 *     void start() override {
 *         std::cout << "MyJob is running!" << std::endl;
 *     }
 * };
 *
 * MyJob job;
 * pool.addJob(job);
 *
 * // The worker pool will automatically execute the jobs
 * @endcode
 */
class WorkerPool
{

private:
    size_t                            _size = 0;
    std::vector<std::thread>          _workers;
    std::queue<std::function<void()>> _jobs;
    std::mutex                        _mtx;
    std::condition_variable           _cv;
    bool                              _stop = false;

public:
    /**
     * @brief Interface for defining jobs to be executed by the worker pool.
     * Classes implementing this interface must define the start() method.
     *
     * @note Not very useful in C++11 and later due to std::function support,
     *      but the subject asked for it.
     */
    class IJobs
    {
    private:
    public:
        virtual ~IJobs() {};
        virtual void start() = 0;
    };

    WorkerPool(const int& nbWorkers);
    ~WorkerPool();

    void loop();
    void addJob(const std::function<void()>& jobToExecute);
    void addJob(IJobs& jobToExecute);
};
#endif