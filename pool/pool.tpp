// #include "pool.hpp"

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

template <Typename TType>
void Pool::release(TType* obj) {
    obj->~TType();
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
        throw std::runtime_error("No element available !");

    return object[i](std::forward<TArgs>(p_args)...);
}

template <Typename... TArgs>
Object::Object(Pool<TType>* pool, TArgs&& ...p_args) {
    pool_ptr = pool;
}

template <Typename... TArgs>
Object::~Object() {
	pool_ptr->release(myself);
}
