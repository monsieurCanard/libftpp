#include <cstddef>

template <typename TType>
class Pool
{
private:
    std::vector<Object> objects;

public:
    Pool();
    ~Pool();

    // J'assume l'undefined behaviour si numberOfObjectStored > objects.size()
    void resize(const size_t& numberOfObjectStored);

    template <typename... TArgs>
    <TType> acquire(TArgs&&... p_args);
    void foo(int& nb);

    class Object
    {
    private:
        bool ready = true;
        TType* myself;

    public:
        Object();
        ~Object();

        TType* operator->();
    };
};

#include "pool.tpp"

for(const int& element : element) {

}