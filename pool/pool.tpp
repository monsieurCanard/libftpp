#include "pool.hpp"

template <Typename TType>
Pool::Pool()
{
    objects.resize(15);
}

template <Typename TType>
Pool::~Pool()
{
}

/*
 * @brief Allocate n objects in the Pool
 * @warning Grow Only Pool(allocate only when arg > objects.size())
 */
void Pool::resize(const size_t& numberOfObjectStored)
{
    if (numberOfObjectStored < objects.size())
        return;
    objects.resize(numberOfObjectStored);
}

template <Typename... TArgs>
Pool::Object<TType> Pool::acquire(TArgs&&... p_args) {
    for(size_t i = 0; i < objects.size(); i++) {
        if(objects[i].ready == true) {
            objects[i].ready = false;
            break;
        }
    }
    if(i == objects.size())
        throw std::runtime_error("No Object Free !");

    return object[i](std::forward<TArgs>(p_args)...);
}

class Object {
private:
public:
    Object();
    ~Object();
    