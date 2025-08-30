#ifndef POOL_HPP
#define POOL_HPP

#include <cstddef>
#include <stack>
#include <stdexcept>
#include <vector>

/**

 * @class Pool
 * @brief Pool of reusable memory.
 *
 * The Pool pre-allocates objects of type @p TType
 * and reuses them to avoid frequent allocations
 * and deallocations.
 *
 * @tparam TType Type of objects managed by the pool.

 */
template <typename TType>
class Pool
{
    class Object
    {
        friend class Pool<TType>;

    private:
        Pool<TType>*                                                       _pool_ptr;
        size_t                                                             _idx;
        typename std::aligned_storage<sizeof(TType), alignof(TType)>::type _myself;

    public:
        Object();
        ~Object();

        template <typename... TArgs>
        Object(Pool<TType>* pool, size_t& index, TArgs&&... p_args);

        /**
         * @brief Return a pointer to the underlying TType object.
         */
        TType* operator->();
        TType* get();
    };

private:
    class Object;

    std::vector<Object> _objects;
    std::stack<size_t>  _available;

public:
    Pool();
    ~Pool();

    /**

     * @brief Pre-allocates memory for a given number of objects.
     * @param numberOfObjectStored Number of objects to pre-allocate.
     * @see acquire()
     * @see release()

     */
    void resize(const size_t& numberOfObjectStored);

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
     * @see release()

     * */
    template <typename... TArgs>
    Object acquire(TArgs&&... p_args);

    /**

     * @brief Releases an object back into the pool.
     *
     * Destroys the contained TType object and return its slot to the available Stack.
     *
     * @param obj Pool::Object to release.
     * @see acquire()

     */
    void release(Object& obj);

    void setAvailable(size_t& idx);
};

#include "pool.tpp"

#endif