#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include <stdexcept>

template <typename TType>
class Singleton
{
private:
    static std::unique_ptr<TType> _instance;

public:
    TType* instance()
    {
        if (!_instance)
            return nullptr;

        return _instance.get();
    }

    template <typename... TArgs>
    void instantiate(TArgs... p_args)
    {
        if (_instance != nullptr)
            throw std::runtime_error("Double Instantiate !");

        _instance = std::make_unique<TType>((p_args)...);
    }

    void reset()
    {
        _instance = nullptr;
    }
};

template <typename TType>
std::unique_ptr<TType> Singleton<TType>::_instance = nullptr;

#endif