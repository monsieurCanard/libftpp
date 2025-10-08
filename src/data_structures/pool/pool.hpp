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
public:
    class Object
    {
        friend class Pool<TType>;

    private:
        Pool<TType>*                                          _pool_ptr;
        int                                                   _idx;
        std::aligned_storage_t<sizeof(TType), alignof(TType)> _myself;

        template <typename... TArgs>
        Object(Pool<TType>* pool, int& index, TArgs&&... p_args);

    public:
        virtual ~Object();
        Object();
        TType* operator->();
        TType* get();
    };

    Pool(const size_t&& size);
    Pool()  = default;
    ~Pool() = default;

    /**
     * @brief Pre-allocates memory for a given number of objects.
     * @param numberOfObjectStored Number of objects to pre-allocate.
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
     * @throw std::out_of_range if no objects are available in the pool.
     * */
    template <typename... TArgs>
    Object acquire(TArgs&&... p_args);

    /**
     * @brief Releases an object back into the pool.
     * Destroys the contained TType object and return its slot to the available Stack.
     */
    void release(Object& obj);

private:
    std::vector<Object> _objects;
    std::stack<int>     _available;
};

#include "pool.tpp"

#endif