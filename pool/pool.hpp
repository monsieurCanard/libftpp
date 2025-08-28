#pragma once

#include <cstddef>
#include <stack>
#include <stdexcept>
#include <vector>

template <typename TType>
class Pool
{
    class Object
    {
        friend class Pool<TType>;

    private:
        Pool<TType>*                                        pool_ptr;
        size_t                                              idx;
        std::aligned_storage<sizeof<TType>, alignof(TType)> myself;

    public:
        Object();
        ~Object();
        template <typename... TArgs>
        Object(Pool<TType>* pool, TArgs&&... p_args);

        TType* operator->();
    };

private:
    class Object;
    std::vector<Object> objects;
    std::stack<size_t>  available;

public:
    Pool();
    ~Pool();

    void resize(const size_t& numberOfObjectStored);

    template <typename... TArgs>
    Object acquire(TArgs&&... p_args);

    void release(Object& obj);
};

#include "pool.tpp"