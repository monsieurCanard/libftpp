#include "pool.hpp"

template <typename TType>
Pool<TType>::Pool()
{
}

template <typename TType>
Pool<TType>::~Pool()
{
}

/*
 * @brief Allocate n objects in the Pool
 * @warning Grow Only Pool(allocate only when arg > objects.size())
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
    obj.myself.~TType();
    std::cout << "Destructor of Obj called" << obj.idx << std::endl;
    available.push(obj.idx);
    std::cout << "kajqdwhgljahdwh of Obj called" << std::endl;
}

template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
    if (available.empty())
        throw std::runtime_error("No Object available !");
    size_t i = available.top();
    new (&objects[i].myself) TType(std::forward<TArgs&&>(p_args)...);
    available.pop();
    pool_ptr       = this;
    objects[i].idx = i;
    return objects[i];
}

template <typename TType>
Pool<TType>::Object::Object() : pool_ptr(nullptr), idx(0)
{
}

template <typename TType>
template <typename... TArgs>
Pool<TType>::Object::Object(Pool<TType>* pool, TArgs&&... p_args)
    : pool_ptr(pool), myself(std::forward<TArgs&&>(p_args)...)
{
}

template <typename TType>
Pool<TType>::Object::~Object()
{
    pool_ptr->release(*this);
}

template <typename TType>
TType* Pool<TType>::Object::operator->()
{
    return &myself;
}
