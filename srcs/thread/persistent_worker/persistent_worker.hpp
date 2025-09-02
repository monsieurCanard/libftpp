#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include <functional>
#include <mutex>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>

#define PAUSE_BT_TASK 0 // ms

class PersistentWorker
{
private:
    std::mutex                                             _mtx;
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