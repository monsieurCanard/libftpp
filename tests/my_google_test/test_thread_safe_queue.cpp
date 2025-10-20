// tests/test_thread_safe_queue.cpp
// g++/clang++ with -pthread, linked with gtest_main.
// Ce fichier ne définit pas de main (utiliser gtest_main).

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <numeric> // pour std::accumulate
#include <set>
#include <thread>
#include <vector>

// Inclure le header de ta queue thread-safe : adapte le chemin si besoin.
#include "libftpp.hpp" // <-- change this if your header / class name is different

using namespace std::chrono_literals;

TEST(ThreadSafeQueue_BasicOperations_Int, PushBackThenPopFrontAndBack)
{
    ThreadSafeQueue<int> q;

    q.push_back(1);
    q.push_back(2);
    q.push_back(3);

    // FIFO from front
    EXPECT_EQ(q.pop_front(), 1);
    EXPECT_EQ(q.pop_front(), 2);

    // remaining element should be 3, can pop from back too
    EXPECT_EQ(q.pop_back(), 3);
}

TEST(ThreadSafeQueue_PushFrontAndMix, PushFrontPushBackPopSequence)
{
    ThreadSafeQueue<int> q;

    // desired final order: 5, 10, 20
    q.push_front(10);
    q.push_back(20);
    q.push_front(5);

    EXPECT_EQ(q.pop_front(), 5);  // first
    EXPECT_EQ(q.pop_back(), 20);  // last
    EXPECT_EQ(q.pop_front(), 10); // middle / remaining
}

TEST(ThreadSafeQueue_StringTypes, WorksWithNonPOD)
{
    ThreadSafeQueue<std::string> q;

    q.push_back("hello");
    q.push_front("first");
    q.push_back(std::string("world"));

    EXPECT_EQ(q.pop_front(), "first");
    EXPECT_EQ(q.pop_front(), "hello");
    EXPECT_EQ(q.pop_back(), "world");
}

TEST(ThreadSafeQueue_EmptyPopThrows, PopFromEmptyThrows)
{
    ThreadSafeQueue<int> q;

    // L'énoncé demande : "must throw an exception" -> on vérifie qu'une exception est levée.
    EXPECT_ANY_THROW(q.pop_front());
    EXPECT_ANY_THROW(q.pop_back());
}

TEST(ThreadSafeQueue_ConcurrentPushPopManyThreads, StressTestProducersConsumers)
{
    ThreadSafeQueue<int> q;

    const int producers          = 6;
    const int consumers          = 3;
    const int items_per_producer = 200;
    const int total              = producers * items_per_producer;

    std::atomic<int>       popped{0};
    std::atomic<long long> sum{0};

    // Producers
    std::vector<std::thread> prod_threads;
    for (int p = 0; p < producers; ++p)
    {
        int base = p * items_per_producer;
        prod_threads.emplace_back(
            [base, &q]()
            {
                for (int i = 0; i < items_per_producer; ++i)
                {
                    q.push_back(base + i);
                }
            });
    }

    // Consumers
    std::vector<std::thread> cons_threads;
    for (int c = 0; c < consumers; ++c)
    {
        cons_threads.emplace_back(
            [&]()
            {
                while (popped.load() < total)
                {
                    try
                    {
                        int v = q.pop_front();
                        sum.fetch_add(v);
                        popped.fetch_add(1);
                    }
                    catch (...)
                    {
                        std::this_thread::sleep_for(1ms);
                    }
                }
            });
    }

    for (auto& t : prod_threads)
        t.join();
    // wait for consumers to finish with timeout
    const auto deadline = std::chrono::steady_clock::now() + 5s;
    while (popped.load() < total && std::chrono::steady_clock::now() < deadline)
    {
        std::this_thread::sleep_for(1ms);
    }

    for (auto& t : cons_threads)
        if (t.joinable())
            t.join();

    EXPECT_EQ(popped.load(), total);

    // Check sum equals expected arithmetic sums
    long long expected_sum = 0;
    for (int p = 0; p < producers; ++p)
    {
        int start = p * items_per_producer;
        int end   = start + items_per_producer;
        expected_sum += (long long)(start + end - 1) * items_per_producer / 2;
    }
    EXPECT_EQ(sum.load(), expected_sum);
}
