#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include <stdexcept>

template <typename TType>
class Singleton
{
private:
    //
    // Note : Utiliser inline pour éviter les erreurs de linkage
    // quand ce header est inclus dans plusieurs fichiers .cpp
    //
    static inline std::unique_ptr<TType> _instance = nullptr;

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
            throw std::runtime_error("Instance already exists.");

        _instance = std::make_unique<TType>((p_args)...);
    }

    void reset()
    {
        _instance = nullptr;
    }
};

#endif