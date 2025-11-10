#include <gtest/gtest.h>

#include <thread> // pour std::this_thread::sleep_for

#include "libftpp.hpp"

class ChronometreTest : public ::testing::Test
{
protected:
    Chronometre chrono;
};

// --- Tests basiques ---
TEST_F(ChronometreTest, StartEndRecordsTime)
{
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    chrono.end();

    double elapsedMs = chrono.getTimeMilliseconds();
    EXPECT_GE(elapsedMs, 9.0);  // devrait être au moins ~10 ms
    EXPECT_LE(elapsedMs, 50.0); // tolérance
}

TEST_F(ChronometreTest, UnitsAreConsistent)
{
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    chrono.end();

    double ns = chrono.getTimeNanoseconds();
    double us = chrono.getTimeMicroseconds();
    double ms = chrono.getTimeMilliseconds();
    double s  = chrono.getTimeSeconds();

    // Cohérence des unités
    EXPECT_NEAR(us, ns / 1000.0, 1e-3);
    EXPECT_NEAR(ms, us / 1000.0, 1e-3);
    EXPECT_NEAR(s, ms / 1000.0, 1e-3);
}

TEST_F(ChronometreTest, TimeStringIsNotEmpty)
{
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    chrono.end();

    std::string result = chrono.getTimeString();
    EXPECT_FALSE(result.empty());
}

// --- Test popLastChrono ---
TEST_F(ChronometreTest, PopLastChronoRemovesLast)
{
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    chrono.end();

    double before = chrono.getTimeMilliseconds();
    EXPECT_GT(before, 0.0);

    chrono.popLastChrono();

    // Si la classe supprime bien la dernière mesure,
    // on s’attend à une erreur ou une valeur spéciale
    EXPECT_THROW(chrono.getTimeMilliseconds(), std::logic_error);
}

// --- Test erreurs ---
TEST_F(ChronometreTest, EndWithoutStartThrows)
{
    Chronometre c;
    EXPECT_THROW(c.end(), std::logic_error);
}
