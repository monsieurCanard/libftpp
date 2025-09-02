#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "libftpp.hpp"

using namespace std::chrono_literals;

// Classe dérivée de WorkerPool::IJobs pour les tests
class TestJob : public WorkerPool::IJobs
{
public:
    std::atomic<int>& counter;
    TestJob(std::atomic<int>& c) : counter(c) {}
    void start() override
    {
        counter++;
    }
};

// Test avec std::function
TEST(WorkerPoolTest, FunctionJobExecution)
{
    std::atomic<int> counter{0};
    WorkerPool       pool(3);

    pool.addJob([&]() { counter++; });

    pool.stopJob(); // s'assurer que tous les threads ont fini
    EXPECT_EQ(counter.load(), 3);
}

// // Test avec IJobs
// TEST(WorkerPoolTest, IJobExecution)
// {
//     std::atomic<int> counter{0};
//     TestJob          job(counter);
//     WorkerPool       pool(4);

//     pool.addJob(job);
//     pool.stopJob();
//     EXPECT_EQ(counter.load(), 4);
// }

// Test que stopJob peut être appelé plusieurs fois
TEST(WorkerPoolTest, StopJobIdempotent)
{
    std::atomic<int> counter{0};
    WorkerPool       pool(2);

    pool.addJob([&]() { counter++; });
    pool.stopJob();

    EXPECT_NO_THROW(pool.stopJob());
    EXPECT_EQ(counter.load(), 2);
}

// Test exécution multiple jobs successifs
TEST(WorkerPoolTest, MultipleAddJobCalls)
{
    std::atomic<int> counter{0};
    WorkerPool       pool(2);

    pool.addJob([&]() { counter++; });
    pool.stopJob();

    pool.addJob([&]() { counter += 2; });
    pool.stopJob();

    EXPECT_EQ(counter.load(), 6); // 2 threads * (1+2)
}
