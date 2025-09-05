#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

template <typename TEvent>
class Observer
{
private:
    std::unordered_map<TEvent, std::vector<std::function<void()>>> _events;

public:
    void subscribe(const TEvent& event, const std::function<void()>& lambda)
    {
        _events[event].push_back(lambda);
    }

    void notify(const TEvent& event)
    {
        typename std::unordered_map<TEvent, std::vector<std::function<void()>>>::iterator it =
            _events.find(event);

        // We can also use auto to simplify the code
        // auto it = _events.find(event);
        if (it == _events.end())
        {
            throw std::runtime_error("No event found");
        }
        for (const auto& funct : it->second)
            funct();
    }
};
#endif