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

    EXPECT_THROW(obs.notify(99), std::runtime_error);
}

TEST(ObserverTest, OverwriteLambda)
{
    Observer<int> obs;
    int           value = 0;

    obs.subscribe(1, [&]() { value = 10; });
    obs.subscribe(1, [&]() { value = 20; }); // remplace la précédente

    obs.notify(1);
    EXPECT_EQ(value, 20); // vérifie que la lambda a bien été remplacée
}

TEST(ObserverTest, MultipleEventsAndLambdas)
{
    Observer<int> obs;
    int           a = 0, b = 0, c = 0;

    obs.subscribe(1, [&]() { a += 1; });
    obs.subscribe(2, [&]() { b += 2; });
    obs.subscribe(1, [&]() { c += 3; }); // remplace la précédente lambda pour 1

    obs.notify(1);
    EXPECT_EQ(a, 0); // l’ancienne lambda pour 1 a été remplacée
    EXPECT_EQ(c, 3);

    obs.notify(2);
    EXPECT_EQ(b, 2);
}

TEST(ObserverTest, LambdaCaptures)
{
    Observer<int> obs;
    int           x = 0;

    obs.subscribe(1, [x]() mutable { x += 5; }); // capture par valeur
    obs.notify(1);
    EXPECT_EQ(x, 0); // capture par valeur → x externe inchangé

    int y = 0;
    obs.subscribe(2, [&y]() { y += 10; }); // capture par référence
    obs.notify(2);
    EXPECT_EQ(y, 10); // y modifié correctement
}

TEST(ObserverTest, DifferentEventTypes)
{
    Observer<std::string> obs;
    std::string           s;

    obs.subscribe("click", [&]() { s = "clicked"; });
    obs.notify("click");
    EXPECT_EQ(s, "clicked");

    Observer<char> obsChar;
    char           c = 'a';
    obsChar.subscribe('X', [&]() { c = 'Z'; });
    obsChar.notify('X');
    EXPECT_EQ(c, 'Z');
}

TEST(ObserverTest, NotifyUnknownEventThrows)
{
    Observer<int> obs;
    obs.subscribe(1, []() {});

    EXPECT_THROW(obs.notify(2), std::runtime_error);
}
