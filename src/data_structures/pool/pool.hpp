#ifndef POOL_HPP
#define POOL_HPP

#include <cstddef>
#include <stack>
#include <stdexcept>
#include <vector>

/**

 * @class Pool
 * @brief Pool of reusable memory.
 * @details RAII is used to manage the lifetime of objects.
 *
 * The Pool pre-allocates objects of type @p TType
 * and reuses them to avoid frequent allocations
 * and deallocations.
 *
 * @warning If you resize the pool, all existing objects are destroyed.
 *
 * @tparam TType Type of objects managed by the pool.
 *
 *
 * @code
 * Pool<MyClass> pool(10); // Create a pool with 10 pre-allocated MyClass objects
 *
 * auto& obj1 = pool.acquire(arg1, arg2); // Acquire an object from the pool
 *
 * obj1->doSomething();
 *
 * pool.release(obj1); // Release the object back to the pool
 *
 * @endcode
 */
template <typename TType>
class Pool
{
public:
    /**
     * @class Object
     * @brief Wrapper around a TType object managed by the Pool.
     * Provides access to the underlying TType object.
     */
    class Object
    {
        friend class Pool<TType>;

    private:
        Pool<TType>*                                          _pool_ptr;
        int                                                   _idx;
        std::aligned_storage_t<sizeof(TType), alignof(TType)> _myself;

        template <typename... TArgs>
        Object(Pool<TType>* pool, int index, TArgs&&... p_args);

    public:
        Object();
        virtual ~Object();

        TType* operator->();
        TType* get();
    };

    Pool(size_t size = 0);
    ~Pool();

    /**
     * @brief Pre-allocates memory for a given number of objects.
     * @param numberOfObjectStored Number of objects to pre-allocate.
     */
    void resize(size_t numberOfObjectStored);

    /**

     * @brief Acquires an object from the pool.
     *
     * Constructs a new TType object in pre-allocated memory
     * and returns a wrapper Object to access it.
     *
     * @tparam TArgs Types of constructor arguments for TType.
     * @param p_args Arguments forwarded to TType's constructor.
     * @return A Pool::Object wrapper containing the allocated TType.
     *
     * @throw std::out_of_range if no objects are available in the pool.
     * */
    template <typename... TArgs>
    Object& acquire(TArgs&&... p_args);

    /**
     * @brief Releases an object back into the pool.
     * Destroys the contained TType object and return its slot to the available Stack.
     */
    void release(Object& obj);

private:
    std::stack<int>     _available;
    std::vector<Object> _objects;
};

#include "pool.tpp"

#endif