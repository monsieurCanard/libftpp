#include <gtest/gtest.h>

#include <string>

#include "libftpp.hpp"

TEST(ObserverTest, SingleEventLambda)
{
    Observer<int> obs;
    int           called = 0;

    obs.subscribe(42, [&]() { called = 1; });
    obs.notify(42);

    EXPECT_EQ(called, 1);
}

TEST(ObserverTest, EventWithString)
{
    Observer<std::string> obs;
    std::string           result;

    obs.subscribe("click", [&]() { result = "clicked"; });
    obs.notify("click");

    EXPECT_EQ(result, "clicked");
}

TEST(ObserverTest, NotifyNonexistentEventThrows)
{
    Observer<int> obs;

    EXPECT_NO_THROW(obs.notify(99));
}

TEST(ObserverTest, OverwriteLambda)
{
    Observer<int> obs;
    int           value = 0;

    obs.subscribe(1, [&]() { value = 10; });
    obs.subscribe(1, [&]() { value = 20; });

    obs.notify(1);
    EXPECT_EQ(value, 20);
}

TEST(ObserverTest, DifferentEventTypes)
{
    Observer<std::string> obs;
    std::string           s;

    obs.subscribe("click", [&]() { s = "clicked"; });
    obs.notify("click");

    Observer<char> obsChar;
    char           c = 'a';
    obsChar.subscribe('X', [&]() { c = 'Z'; });
    obsChar.notify('X');

    EXPECT_EQ(s, "clicked");
    EXPECT_EQ(c, 'Z');
}

TEST(ObserverTest, NotifyUnknownEventThrows)
{
    Observer<int> obs;

    EXPECT_NO_THROW(obs.notify(2));
}
