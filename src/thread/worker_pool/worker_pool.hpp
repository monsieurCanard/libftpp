#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

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