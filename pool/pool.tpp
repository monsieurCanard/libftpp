#include "pool.hpp"

template <typename TType>
Pool<TType>::Pool()
{
}

template <typename TType>
Pool<TType>::~Pool()
{
#ifdef DEBUG // Just For 42 Eval
    std::cout << "Pool Destructor Call" << std::endl;
#endif
}

/*
 * @brief Resize the pool to contain n objects
 * @warning If n is less than the current size, objects will be lost
 * @param numberOfObjectStored Number of objects to allocate
 */
template <typename TType>
void Pool<TType>::resize(const size_t& numberOfObjectStored)
{
    objects.resize(numberOfObjectStored);
    for (size_t i = 0; i < numberOfObjectStored; i++)
    {
        available.push(i);
    }
}

template <typename TType>
void Pool<TType>::release(Object& obj)
{
    obj.get()->~TType();
    available.push(obj.idx);
}

template <typename TType>
void Pool<TType>::setAvailable(size_t& index)
{
    this->available.push(index);
}

template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
    if (available.empty())
        throw std::runtime_error("No Object available !");

    size_t i = available.top();
    available.pop();

    // TType* ptr = new (&objects[i].myself) TType(std::forward<TArgs&&>(p_args)...);
    return Object(this, i, std::forward<TArgs&&>(p_args)...);
}

template <typename TType>
Pool<TType>::Object::Object() : pool_ptr(nullptr), idx(0)
{
}

template <typename TType>
template <typename... TArgs>
Pool<TType>::Object::Object(Pool<TType>* pool, size_t& index, TArgs&&... p_args)
    : pool_ptr(pool), idx(index)
{
    new (&myself) TType(std::forward<TArgs&&>(p_args)...);
}

template <typename TType>
Pool<TType>::Object::~Object()
{
    if (pool_ptr != nullptr)
        pool_ptr->release(*this);

#ifdef DEBUG // Just For 42 Eval
    if (pool_ptr != nullptr)
        std::cout << "Release Object Called" << std::endl;
#endif
}

template <typename TType>
TType* Pool<TType>::Object::operator->()
{
    return get();
}

template <typename TType>
TType* Pool<TType>::Object::get()
{
    return reinterpret_cast<TType*>(&myself);
}
