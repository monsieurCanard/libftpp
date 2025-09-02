#include <gtest/gtest.h>

#include "libftpp.hpp"

// --- Test struct custom pour la généralité ---
struct TestStruct
{
    int x, y;
    TestStruct(int a = 0, int b = 0) : x(a), y(b) {}
    void increment()
    {
        x++;
        y++;
    }
};

// --- Test acquisition simple ---
TEST(PoolTest, AcquireSingleInt)
{
    Pool<int> pool;
    pool.resize(2);

    auto obj = pool.acquire(42); // constructeur int
    EXPECT_EQ(*obj.get(), 42);

    (*obj.get())++; // utiliser l'operator->()
    EXPECT_EQ(*obj.get(), 43);
}

// --- Test acquisition multiple et release ---
TEST(PoolTest, AcquireReleaseReuse)
{
    Pool<int> pool;
    pool.resize(2);

    auto obj1 = pool.acquire(1);
    auto obj2 = pool.acquire(2);

    EXPECT_EQ(*obj1.get(), 1);
    EXPECT_EQ(*obj2.get(), 2);

    pool.release(obj1);           // libère le premier objet
    auto obj3 = pool.acquire(10); // doit réutiliser le slot d'obj1

    EXPECT_EQ(*obj3.get(), 10);
}

// --- Test resize pré-allocation ---
TEST(PoolTest, ResizeAndAvailable)
{
    Pool<int> pool;
    pool.resize(5);

    size_t count = 0;
    for (size_t i = 0; i < 5; ++i)
    {
        auto obj = pool.acquire(static_cast<int>(i));
        EXPECT_EQ(*obj.get(), static_cast<int>(i));
        count++;
    }

    EXPECT_EQ(count, 5);
}

// --- Test avec struct custom ---
TEST(PoolTest, AcquireCustomStruct)
{
    Pool<TestStruct> pool;
    pool.resize(2);

    auto obj = pool.acquire(3, 4); // TestStruct(3,4)
    EXPECT_EQ(obj->x, 3);
    EXPECT_EQ(obj->y, 4);

    obj->increment();
    EXPECT_EQ(obj->x, 4);
    EXPECT_EQ(obj->y, 5);
}

// --- Test multiple acquires/releases ---
TEST(PoolTest, MultipleAcquireRelease)
{
    Pool<int> pool;
    pool.resize(3);

    auto a = pool.acquire(1);
    auto b = pool.acquire(2);
    auto c = pool.acquire(3);

    pool.release(a);
    pool.release(b);

    auto d = pool.acquire(10);
    auto e = pool.acquire(20);

    EXPECT_EQ(*d.get(), 10);
    EXPECT_EQ(*e.get(), 20);

    EXPECT_EQ(*c.get(), 3); // objet jamais relâché reste intact
}

// --- Test exception ou comportement si trop d'acquisitions ---
TEST(PoolTest, OverAcquire)
{
    Pool<int> pool;
    pool.resize(2);

    auto a = pool.acquire(1);
    auto b = pool.acquire(2);

    // Selon ton implémentation, essayer d'acquérir un 3ème objet peut échouer.
    // Si tu throws exception, tu peux tester :
    EXPECT_THROW(pool.acquire(3), std::runtime_error);
}
