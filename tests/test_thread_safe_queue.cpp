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

TEST(ThreadSafeQueue_ThreadSafety_SimpleProducerConsumer, MultipleProducersSingleConsumer)
{
    ThreadSafeQueue<int> q;

    const int producers_count    = 4;
    const int items_per_producer = 1000;
    const int total_items        = producers_count * items_per_producer;

    // We'll push distinct ranges so we can verify the sum at the end
    std::vector<std::thread> producers;
    producers.reserve(producers_count);

    long long expected_sum = 0;
    for (int p = 0; p < producers_count; ++p)
    {
        int start = p * items_per_producer;
        int end   = start + items_per_producer;
        // sum of arithmetic sequence
        long long sum_range = (long long)(start + end - 1) * items_per_producer / 2;
        expected_sum += sum_range;

        producers.emplace_back(
            [start, end, &q]()
            {
                for (int v = start; v < end; ++v)
                {
                    q.push_back(v);
                }
            });
    }

    std::atomic<int>       popped_count{0};
    std::atomic<long long> popped_sum{0};

    // Consumer thread: keep trying to pop until it collected total_items.
    std::thread consumer(
        [&]()
        {
            while (popped_count.load() < total_items)
            {
                try
                {
                    int v = q.pop_front();
                    popped_sum.fetch_add(v);
                    popped_count.fetch_add(1);
                }
                catch (...)
                {
                    // queue might be temporarily empty -> yield and retry
                    std::this_thread::sleep_for(1ms);
                }
            }
        });

    for (auto& t : producers)
        t.join();
    // Wait for consumer to finish with a timeout guard to avoid infinite hang in test
    const auto max_wait   = 5s;
    const auto start_wait = std::chrono::steady_clock::now();
    while (popped_count.load() < total_items)
    {
        if (std::chrono::steady_clock::now() - start_wait > max_wait)
            break;
        std::this_thread::sleep_for(1ms);
    }

    // If consumer still running, join (it should exit if it popped all)
    if (consumer.joinable())
        consumer.join();

    EXPECT_EQ(popped_count.load(), total_items);
    EXPECT_EQ(popped_sum.load(), expected_sum);
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
                    // small optional pause to increase interleaving
                    if ((i & 31) == 0)
                        std::this_thread::yield();
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
