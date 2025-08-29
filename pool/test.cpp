#include <iostream>
#include <string>

#include "pool.hpp"

class Canard
{
public:
    int         value;
    std::string name;

    Canard(int v, std::string n) : value(v), name(n)
    {
        std::cout << "Constructed Canard(" << value << ", " << name << ")\n";
    }

    ~Canard()
    {
        std::cout << "Destroyed Canard(" << value << ", " << name << ")\n";
    }

    void showObject()
    {
        std::cout << "Object " << name << " has value " << value << "\n";
    }
};

int main()
{
    Pool<Canard> pool;

    pool.resize(3);
    std::cout << "Pool resized to 3 objects\n";

    auto obj1 = pool.acquire(1, "Albert");
    obj1->showObject();

    auto obj2 = pool.acquire(20, "Bertrand");
    obj2->showObject();

    {
        auto obj3 = pool.acquire(30, "Carole");
        obj3->showObject();
    }

    auto obj4 = pool.acquire(40, "Davitrand");
    obj4->showObject();

    try
    {
        auto obj5 = pool.acquire(7777, "Esteban");
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Exception captured: " << e.what() << "\n";
    }

    std::cout << "End of main, remaining objects will be destroyed\n";
    std::cout << std::endl;

    return 0;
}
