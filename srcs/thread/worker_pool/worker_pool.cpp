#include "worker_pool.hpp"
WorkerPool::WorkerPool(const int& nbWorkers)
{
    _workers.reserve(nbWorkers);
    _size = nbWorkers;
}

WorkerPool::~WorkerPool()
{
    stopJob();
}

void WorkerPool::addJob(const std::function<void()>& jobToExecute)
{
    for (size_t i = 0; i < _size; i++)
    {
        _workers.push_back(std::thread(jobToExecute));
    }
}

void WorkerPool::stopJob()
{
    for (size_t i = 0; i < _size; i++)
    {
        if (_workers[i].joinable())
            _workers[i].join();
    }
    _workers.clear();
}