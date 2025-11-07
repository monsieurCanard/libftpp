#ifndef OBSERVABLE_VALUE_HPP
#define OBSERVABLE_VALUE_HPP

#include <memory>
#include <set>

#include "IObserver/IObserver.hpp"

/**
 * @brief ObservableValue class template that implements the Observer design pattern.
 *        It allows observers to subscribe and be notified when the value changes.
 *
 * @tparam TType The type of the value being observed.
 * @note If ObservableValue is destroyed, every observer is unsubscribed automatically.
 */
template <typename TType>
class ObservableValue
{
private:
    TType                       _value;
    std::set<IObserver<TType>*> _subscriber;

    void notify() const
    {
        for (auto* observer : _subscriber)
        {
            if (observer)
                observer->update(_value);
        }
    }

public:
    ObservableValue() = default;
    ~ObservableValue()
    {
        for (auto* observer : _subscriber)
        {
            unsubscribe(observer);
        }
    }

    ObservableValue(TType value)
    {
        _value = std::move(value);
    }

    ObservableValue& operator=(const TType& newValue)
    {
        if (newValue != _value)
            set(newValue);

        return *this;
    }

    void subscribe(IObserver<TType>* observer)
    {
        if (observer)
            _subscriber.insert(observer);
    }

    void unsubscribe(IObserver<TType>* observer)
    {
        if (observer)
            _subscriber.erase(observer);
    }

    void set(TType newValue)
    {
        if (newValue != _value)
        {
            _value = std::move(newValue);
            notify();
        }
    }

    int countSubscriber() const
    {
        return _subscriber.size();
    }

    const TType& get() const
    {
        return _value;
    }
};
#endif