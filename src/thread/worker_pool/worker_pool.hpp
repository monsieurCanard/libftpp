#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include <functional>
#include <thread>
#include <vector>

class WorkerPool
{

private:
    size_t                   _size = 0;
    std::vector<std::thread> _workers;

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
    void addJob(const std::function<void()>& jobToExecute);
    void stopJob();
};
#endif