#include <cstddef>

template <typename TType>
class Pool
{
private:
    std::vector<Object> objects;

public:
    Pool();
    ~Pool();

    void resize(const size_t& numberOfObjectStored);

    template <typename... TArgs>
    <TType> acquire(TArgs&&... p_args);
    
    void release(*TType);

    class Object
    {
    private:
        Pool<TType>* pool_ptr; 
        bool ready = true;
        TType* myself;
    public:
        template <Typename... TArgs>
        Object(Pool<TType>* pool, TArgs&&... p_args);
        ~Object();

        TType* operator->();
    };
};

#include "pool.tpp"