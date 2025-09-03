#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <mutex>
#include <queue>

template <typename TType>
class ThreadSafeQueue
{
private:
    std::deque<TType> _queue;
    std::mutex        _mutex;

public:
    void push_back(const TType& newElement)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push_back(newElement);
    }
    void push_front(const TType& newElement)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push_front(newElement);
    }

    // Debattre de l'argument const pour pop back qui ne sert a rien
    TType pop_back()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_queue.empty())
            throw std::runtime_error("Trying to pop from an empty queue");

        TType value = _queue.back();
        _queue.pop_back();
        return value;
    }

    TType pop_front()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_queue.empty())
            throw std::runtime_error("Trying to pop from an empty queue");

        TType value = _queue.front();
        _queue.pop_front();
        return value;
    }
};
#endif