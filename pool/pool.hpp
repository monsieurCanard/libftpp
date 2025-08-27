#include <cstddef>

class Pool
{
private:
public:
    void resize(const size_t& numberOfObjectStored);
    class Object
    {
        bool ready = true;
        template <typename... TArgs>
    }