#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "libftpp.hpp"

using namespace std::chrono_literals;

// Test basique : un seul job
TEST(WorkerPoolTest, SingleJob)
{
    std::atomic<int> counter{0};

    {
        WorkerPool pool(2);
        pool.addJob([&counter]() { counter++; });

        // Attendre un peu pour que le job s'exécute
        std::this_thread::sleep_for(100ms);
    } // Le destructeur arrête automatiquement le pool

    EXPECT_EQ(counter.load(), 1);
}

// Test avec plusieurs jobs
TEST(WorkerPoolTest, MultipleJobs)
{
    std::atomic<int> counter{0};
    const int        numJobs = 10;

    {
        WorkerPool pool(3);

        for (int i = 0; i < numJobs; ++i)
        {
            pool.addJob([&counter]() { counter++; });
        }

        // Attendre que tous les jobs se terminent
        std::this_thread::sleep_for(200ms);
    }

    EXPECT_EQ(counter.load(), numJobs);
}

// Test avec des jobs qui prennent du temps
TEST(WorkerPoolTest, LongRunningJobs)
{
    std::atomic<int> counter{0};
    const int        numJobs    = 6;
    const int        numWorkers = 3;

    auto start = std::chrono::high_resolution_clock::now();

    {
        WorkerPool pool(numWorkers);

        for (int i = 0; i < numJobs; ++i)
        {
            pool.addJob(
                [&counter]()
                {
                    std::this_thread::sleep_for(50ms); // Job qui prend du temps
                    counter++;
                });
        }

        // Attendre que tous les jobs se terminent
        std::this_thread::sleep_for(200ms);
    }

    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_EQ(counter.load(), numJobs);
    // Avec 3 workers et 6 jobs de 50ms chacun, ça devrait prendre ~100ms (2 batches)
    // Plus le temps de setup/cleanup
    EXPECT_LT(duration.count(), 300); // Moins de 300ms au total
}

// Test de destruction propre
TEST(WorkerPoolTest, ProperDestruction)
{
    std::atomic<int>  counter{0};
    std::atomic<bool> jobStarted{false};
    std::atomic<bool> jobFinished{false};

    {
        WorkerPool pool(1);

        pool.addJob(
            [&]()
            {
                jobStarted = true;
                std::this_thread::sleep_for(100ms);
                counter++;
                jobFinished = true;
            });

        // Attendre que le job commence
        while (!jobStarted)
        {
            std::this_thread::sleep_for(1ms);
        }

        // Le destructeur devrait attendre que le job se termine
    }

    EXPECT_TRUE(jobFinished.load());
    EXPECT_EQ(counter.load(), 1);
}

// Test avec des jobs qui lèvent des exceptions
TEST(WorkerPoolTest, ExceptionHandling)
{
    std::atomic<int> successCounter{0};
    std::atomic<int> exceptionCounter{0};

    {
        WorkerPool pool(2);

        // Job qui réussit
        pool.addJob([&successCounter]() { successCounter++; });

        // Job qui lève une exception
        pool.addJob([&exceptionCounter]() { exceptionCounter++; });

        // Autre job qui réussit
        pool.addJob([&successCounter]() { successCounter++; });

        std::this_thread::sleep_for(300ms);
    }

    // Les jobs qui réussissent doivent s'exécuter malgré l'exception
    EXPECT_EQ(successCounter.load(), 2);
    EXPECT_EQ(exceptionCounter.load(), 1);
}

// Test de performance basique
TEST(WorkerPoolTest, Performance)
{
    const int        numJobs    = 1000;
    const int        numWorkers = 4;
    std::atomic<int> counter{0};

    auto start = std::chrono::high_resolution_clock::now();

    {
        WorkerPool pool(numWorkers);

        for (int i = 0; i < numJobs; ++i)
        {
            pool.addJob(
                [&counter]()
                {
                    // Travail minimal
                    counter++;
                });
        }

        // Attendre que tous les jobs se terminent
        while (counter.load() < numJobs)
        {
            std::this_thread::sleep_for(1ms);
        }
    }

    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_EQ(counter.load(), numJobs);
    // 1000 jobs simples devraient se terminer rapidement
    EXPECT_LT(duration.count(), 1000); // Moins de 1 seconde
}

// Test avec différents nombres de workers
TEST(WorkerPoolTest, DifferentWorkerCounts)
{
    std::vector<int> workerCounts = {1, 2, 4, 8};
    const int        numJobs      = 20;

    for (int workers : workerCounts)
    {
        std::atomic<int> counter{0};

        {
            WorkerPool pool(workers);

            for (int i = 0; i < numJobs; ++i)
            {
                pool.addJob([&counter]() { counter++; });
            }

            std::this_thread::sleep_for(200ms);
        }

        EXPECT_EQ(counter.load(), numJobs) << "Failed with " << workers << " workers";
    }
}
