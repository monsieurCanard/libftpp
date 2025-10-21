#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "libftpp.hpp"

// Test que l'on peut ajouter une tâche et qu'elle s'exécute
TEST(PersistentWorkerTest, AddTaskExecutes)
{
    PersistentWorker worker;

    std::atomic<int> counter{0};
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    worker.addTask("increment", [&]() { counter++; });

    // Attendre un peu pour que la tâche s’exécute plusieurs fois
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    worker.removeTask("increment");
    EXPECT_GT(counter.load(), 0); // La tâche a tourné au moins une fois
}

// Test suppression de tâche
TEST(PersistentWorkerTest, RemoveTaskStopsExecution)
{
    PersistentWorker worker;

    std::atomic<int> counter{0};
    worker.addTask("increment", [&]() { counter++; });

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    worker.removeTask("increment");

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    int before = counter.load();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    EXPECT_EQ(before, counter.load()); // plus d'incrémentations après suppression
}

// Test ajout de plusieurs tâches
TEST(PersistentWorkerTest, MultipleTasksRun)
{
    PersistentWorker worker;

    std::atomic<int> counter1{0};
    std::atomic<int> counter2{0};

    worker.addTask("task1", [&]() { counter1++; });
    worker.addTask("task2", [&]() { counter2++; });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_GT(counter1.load(), 0);
    EXPECT_GT(counter2.load(), 0);
}

// Test que le worker se termine proprement à la destruction
TEST(PersistentWorkerTest, DestructorStopsThread)
{
    std::atomic<int> counter{0};
    {
        PersistentWorker worker;
        worker.addTask("increment", [&]() { counter++; });
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    } // worker détruit ici

    int before = counter.load();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    EXPECT_EQ(before, counter.load()); // ne bouge plus après destruction
}
