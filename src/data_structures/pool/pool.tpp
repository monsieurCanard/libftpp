#include "pool.hpp"

template <typename TType>
Pool<TType>::Pool(const size_t&& size)
{
    resize(size);
}

/// --- Implementation of Pool::Object ---
template <typename TType>
template <typename... TArgs>
Pool<TType>::Object::Object(Pool<TType>* pool, int& index, TArgs&&... p_args)
    : _pool_ptr(pool), _idx(index)
{
    new (&_myself) TType(std::forward<TArgs&&>(p_args)...);
}

template <typename TType>
Pool<TType>::Object::~Object()
{
    if (_pool_ptr != nullptr)
        _pool_ptr->release(*this);
}

template <typename TType>
Pool<TType>::Object::Object() : _pool_ptr(nullptr), _idx(0)
{
}

template <typename TType>
void Pool<TType>::resize(const size_t& numberOfObjectStored)
{
    _objects.resize(numberOfObjectStored);
    for (int i = 0; i < static_cast<int>(numberOfObjectStored); ++i)
    {
        _available.push(i);
    }
}

/// --- Implementation of Pool ---
template <typename TType>
void Pool<TType>::release(Object& obj)
{
    if (obj._pool_ptr == nullptr)
        return;

    obj.get()->~TType();
    _available.push(obj._idx);
    obj._pool_ptr = nullptr;
}

template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
    if (_available.empty())
        throw std::out_of_range(
            "No available objects in the pool. Maybe try to increase its size.");

    int i = _available.top();
    _available.pop();

    return Object(this, i, std::forward<TArgs>(p_args)...);
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
