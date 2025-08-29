#include <iostream>
#include <string>

#include "pool.hpp"

class MyObject
{
public:
    int         value;
    std::string name;

    MyObject(int v, std::string n) : value(v), name(n)
    {
        std::cout << "Constructed MyObject(" << value << ", " << name << ")\n";
    }

    ~MyObject()
    {
        std::cout << "Destroyed MyObject(" << value << ", " << name << ")\n";
    }

    void show()
    {
        std::cout << "Object " << name << " has value " << value << "\n";
    }
};

int main()
{
    Pool<MyObject> pool;

    pool.resize(3);
    std::cout << "Pool resized to 3 objects\n";

    auto obj1 = pool.acquire(10, "A");
    obj1->show();

    auto obj2 = pool.acquire(20, "B");
    obj2->show();

    {
        auto obj3 = pool.acquire(30, "C");
        obj3->show();
    }

    auto obj4 = pool.acquire(40, "D");
    obj4->show();

    try
    {
        auto obj5 = pool.acquire(50, "E");
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Exception captured: " << e.what() << "\n";
    }

    // Test 5 : vérifier destruction à la fin du programme
    std::cout << "End of main, remaining objects will be destroyed\n";

    return 0;
}
