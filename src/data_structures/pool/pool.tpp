#include "pool.hpp"

/// --- Implementation of Pool::Object ---
template <typename TType>
Pool<TType>::Object::~Object()
{
    if (_pool_ptr != nullptr)
        _pool_ptr->release(*this);
}

template <typename TType>
Pool<TType>::Object::Object() : _pool_ptr(nullptr), _idx(0)
{
    new (&_myself) TType();
}

/// --- Implementation of Pool ---
template <typename TType>
Pool<TType>::Pool(size_t size)
{
    resize(size);
}

template <typename TType>
Pool<TType>::~Pool()
{
    for (auto& obj : _objects)
    {
        obj.get()->~TType();
    }

    while (!_available.empty())
    {
        _available.pop();
    }
}

template <typename TType>
void Pool<TType>::resize(size_t numberOfObjectStored)
{
    for (auto& obj : _objects)
    {
        obj.get()->~TType();
    }

    while (!_available.empty())
    {
        _available.pop();
    }

    _objects.resize(numberOfObjectStored);

    for (int i = 0; i < static_cast<int>(numberOfObjectStored); ++i)
    {
        _objects[i]._idx = i;
        _available.push(i);
    }
}

template <typename TType>
void Pool<TType>::release(Object& obj)
{
    if (obj._pool_ptr == nullptr)
        return;

    _available.push(obj._idx);
    obj._pool_ptr = nullptr;
}

template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object& Pool<TType>::acquire(TArgs&&... p_args)
{
    if (_available.empty())
        throw std::out_of_range(
            "No available objects in the pool. Maybe try to increase its size.");

    int i = _available.top();
    _available.pop();

    _objects[i]._pool_ptr = this;
    _objects[i].get()->~TType();

    new (&_objects[i]._myself) TType(std::forward<TArgs>(p_args)...);
    return _objects[i];
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
