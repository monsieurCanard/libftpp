#include <gtest/gtest.h>

#include "libftpp.hpp"

// Concrete observer pour les tests
template <typename T>
class TestObserver : public IObserver<T>
{
public:
    int updateCount = 0;
    T   lastValue;

    void update(const T& newValue) override
    {
        updateCount++;
        lastValue = newValue;
    }
};

// Test de base : initialisation et get()
TEST(ObservableValueTest, InitialValue)
{
    ObservableValue<int> obs(10);
    EXPECT_EQ(obs.get(), 10);
    EXPECT_EQ(obs.countSubscriber(), 0u);
}

// Test de subscribe et notification
TEST(ObservableValueTest, SubscribeAndNotify)
{
    ObservableValue<int> obs(5);
    TestObserver<int>    observer;

    obs.subscribe(&observer);
    EXPECT_EQ(obs.countSubscriber(), 1u);

    obs.set(10); // devrait notifier
    EXPECT_EQ(observer.updateCount, 1);
    EXPECT_EQ(observer.lastValue, 10);

    // Changer la valeur à la même valeur : ne doit pas notifier
    obs.set(10);
    EXPECT_EQ(observer.updateCount, 1); // toujours 1
}

// Test de unsubscribe
TEST(ObservableValueTest, Unsubscribe)
{
    ObservableValue<int> obs(0);
    TestObserver<int>    observer;

    obs.subscribe(&observer);
    obs.unsubscribe(&observer);
    EXPECT_EQ(obs.countSubscriber(), 0u);

    obs.set(42); // ne doit pas notifier
    EXPECT_EQ(observer.updateCount, 0);
}

// Test multiple observers
TEST(ObservableValueTest, MultipleObservers)
{
    ObservableValue<std::string> obs("hello");
    TestObserver<std::string>    obs1;
    TestObserver<std::string>    obs2;

    obs.subscribe(&obs1);
    obs.subscribe(&obs2);

    obs.set("world");

    EXPECT_EQ(obs1.updateCount, 1);
    EXPECT_EQ(obs1.lastValue, "world");

    EXPECT_EQ(obs2.updateCount, 1);
    EXPECT_EQ(obs2.lastValue, "world");
}
