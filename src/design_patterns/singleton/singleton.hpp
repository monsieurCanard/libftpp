#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include <stdexcept>

/**
 * @brief Singleton Design Pattern
 *
 * @tparam TType Type of the singleton class
 *
 * @code
 * class MyClass
 * {
 * public:
 *    int         value;
 *    std::string name; *
 *    MyClass() : value(0), name("default") {}
 *    MyClass(int v, const std::string& n) : value(v), name(n) {} *
 *    void someMethod()
 *    {
 *        value += 1;
 *    }
 * };
 *
 * // Usage
 *
 * Singleton<MyClass>::instantiate(42, "hello");
 *
 * MyClass* instance = Singleton<MyClass>::instance();
 * instance->someMethod();
 *
 * // Now instance->value == 43
 * @endcode
 */
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
    static TType* instance()
    {
        if (!_instance)
            throw std::runtime_error("Instance not yet created.");

        return _instance.get();
    }

    template <typename... TArgs>
    static void instantiate(TArgs... p_args)
    {
        if (_instance != nullptr)
            throw std::runtime_error("Instance already exists.");

        _instance = std::make_unique<TType>((p_args)...);
    }

    static void reset()
    {
        _instance = nullptr;
    }
};

#endif