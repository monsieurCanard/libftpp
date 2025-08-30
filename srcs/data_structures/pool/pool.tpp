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
    _objects.resize(numberOfObjectStored);
    for (size_t i = 0; i < numberOfObjectStored; i++)
    {
        _available.push(i);
    }
}

template <typename TType>
void Pool<TType>::release(Object& obj)
{
    obj.get()->~TType();
    _available.push(obj._idx);
}

template <typename TType>
void Pool<TType>::setAvailable(size_t& index)
{
    this->_available.push(index);
}

template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
    if (_available.empty())
        throw std::runtime_error("No Object available !");

    size_t i = _available.top();
    _available.pop();

    return Object(this, i, std::forward<TArgs&&>(p_args)...);
}

template <typename TType>
Pool<TType>::Object::Object() : _pool_ptr(nullptr), _idx(0)
{
}

template <typename TType>
template <typename... TArgs>
Pool<TType>::Object::Object(Pool<TType>* pool, size_t& index, TArgs&&... p_args)
    : _pool_ptr(pool), _idx(index)
{
    new (&_myself) TType(std::forward<TArgs&&>(p_args)...);
}

template <typename TType>
Pool<TType>::Object::~Object()
{
    if (_pool_ptr != nullptr)
        _pool_ptr->release(*this);

#ifdef DEBUG // Just For 42 Eval
    if (_pool_ptr != nullptr)
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
    return reinterpret_cast<TType*>(&_myself);
}
