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

    auto obj2 = pool.acquire(2);
    {
        auto obj1 = pool.acquire(1);

        EXPECT_EQ(*obj1.get(), 1);
        EXPECT_EQ(*obj2.get(), 2);
    }
    // pool.release(obj1);           // libère le premier objet
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

    {
        auto a = pool.acquire(1);
        auto b = pool.acquire(2);
    }
    auto c = pool.acquire(3);

    auto d = pool.acquire(10);
    auto e = pool.acquire(20);

    EXPECT_EQ(*d.get(), 10);
    EXPECT_EQ(*e.get(), 20);

    EXPECT_EQ(*c.get(), 3);
}

// --- Test exception ou comportement si trop d'acquisitions ---
TEST(PoolTest, OverAcquire)
{
    Pool<int> pool;
    pool.resize(2);

    auto a = pool.acquire(1);
    auto b = pool.acquire(2);

    EXPECT_THROW(pool.acquire(3), std::out_of_range);
}

TEST(PoolTest, Thread)
{
    Pool<std::thread> pool;
    pool.resize(2);
    int counter = 0;

    auto threadFunc      = []() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); };
    auto threadIncrement = [](int& counter)
    {
        for (int i = 0; i < 10; ++i)
        {
            counter++;
        }
    };

    {
        auto t1 = pool.acquire(threadIncrement, std::ref(counter));
        EXPECT_EQ(t1.get()->joinable(), true);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        EXPECT_GE(counter, 1);

        auto t2 = pool.acquire(threadFunc);
        EXPECT_EQ(t2.get()->joinable(), true);

        EXPECT_THROW(pool.acquire(threadFunc), std::out_of_range);

        t1->join();
        t2->join();

        EXPECT_EQ(counter, 10);

    } // Les threads sont détruits ici et doivent se terminer correctement
}
