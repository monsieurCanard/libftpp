#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>

#include "libftpp.hpp"

// On utilise chrono pour éviter des boucles infinies
using namespace std::chrono_literals;

TEST(Thread_Basic, StartAndStop) {
    std::atomic<bool> executed{false};

    Thread t("Worker", [&]() {
        executed = true;
    });

    t.start();
    t.stop();

    EXPECT_TRUE(executed.load());
}

TEST(Thread_Basic, StopWithoutStartDoesNothing) {
    Thread t("Idle", []() {
        // ne fait rien
    });

    // ne doit pas planter
    EXPECT_NO_THROW(t.stop());
}

TEST(Thread_Basic, StopMultipleTimesIsSafe) {
    Thread t("SafeStop", []() {
        // court
    });

    t.start();
    t.stop();

    // stop() encore ne doit rien casser
    EXPECT_NO_THROW(t.stop());
}

TEST(Thread_Basic, DoubleStartRelanchesThread) {
    std::atomic<int> counter{0};

    Thread t("Restartable", [&]() {
        counter++;
    });

    t.start();
    t.stop();

    // start() une deuxième fois doit relancer un nouveau thread
    EXPECT_NO_THROW(t.start());
    t.stop();

    EXPECT_EQ(counter.load(), 2);
}

TEST(Thread_Basic, ThreadRunsWorkProperly) {
    std::atomic<int> value{0};

    Thread t("Worker", [&]() {
        // simulation travail
        std::this_thread::sleep_for(50ms);
        value = 42;
    });

    t.start();
    t.stop();

    EXPECT_EQ(value.load(), 42);
}
