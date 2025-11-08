#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <mutex>
#include <queue>
/**
 * @brief Thread-Safe Queue
 *
 * This class provides a thread-safe wrapper around std::deque for
 * managing a queue of elements. It ensures that multiple threads can
 * safely push and pop elements from the queue without data races.
 *
 * @note Uses a mutex to synchronize access to the underlying deque
 * @note Supports both push and pop operations from both ends of the queue
 *
 * @code
 * ThreadSafeQueue<int> queue;
 * queue.push_back(1);
 * int value = queue.pop_front();
 * @endcode
 */
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