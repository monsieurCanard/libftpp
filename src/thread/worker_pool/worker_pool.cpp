#include "worker_pool.hpp"

WorkerPool::WorkerPool(const int& nbWorkers)
{
    _workers.reserve(nbWorkers);
    _size = nbWorkers;
    for (size_t i = 0; i < _size; i++)
    {
        _workers.emplace_back(&WorkerPool::loop, this);
    }
}

void WorkerPool::loop()
{
    while (!_stop)
    {
        std::function<void()> action;
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cv.wait(lock, [&] { return _stop || !_jobs.empty(); });

            if (_stop || _jobs.empty())
                break;

            action = _jobs.front();
            _jobs.pop();
        }
        action();
    }
}

WorkerPool::~WorkerPool()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _stop = true;
    }

    _cv.notify_all();

    for (size_t i = 0; i < _size; i++)
    {
        if (_workers[i].joinable())
            _workers[i].join();
    }

    _workers.clear();

    std::lock_guard<std::mutex> lock(_mtx);
    while (!_jobs.empty())
        _jobs.pop();
}

void WorkerPool::addJob(const std::function<void()>& jobToExecute)
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _jobs.push(jobToExecute);
    }
    _cv.notify_one();
}

void WorkerPool::addJob(IJobs& jobToExecute)
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _jobs.push([&jobToExecute]() { jobToExecute.start(); });
    }
    _cv.notify_one();
}