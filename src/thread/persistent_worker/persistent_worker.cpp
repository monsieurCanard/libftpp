#include "persistent_worker.hpp"

PersistentWorker::PersistentWorker()
{
    _thread = std::thread(&PersistentWorker::loop, this);
}

PersistentWorker::~PersistentWorker()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _running = false;
    }
    if (_thread.joinable())
        _thread.join();
}

void PersistentWorker::addTask(const std::string& name, const std::function<void()>& jobToExecute)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _tasks[name] = jobToExecute;
}

void PersistentWorker::removeTask(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _tasks.erase(name);
}

void PersistentWorker::loop()
{
    std::unordered_map<std::string, std::function<void()>> copy;
    bool                                                   keepGoing = true;
    while (keepGoing)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            copy = _tasks;
        }

        for (std::pair<std::string, std::function<void()>> element : copy)
        {
            element.second();
            std::this_thread::sleep_for(std::chrono::milliseconds(PAUSE_BT_TASK));
        }

        {
            std::lock_guard<std::mutex> lock(_mtx);
            keepGoing = _running;
        }
    }
}