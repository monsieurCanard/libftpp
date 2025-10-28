// test_singleton.cpp
#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "libftpp.hpp"

// Une classe simple pour tester le Singleton
class MyClass
{
public:
    int         value;
    std::string name;

    MyClass() : value(0), name("default") {}
    MyClass(int v, const std::string& n) : value(v), name(n) {}

    void someMethod()
    {
        value += 1;
    }
};

// ---- Tests ----

TEST(SingletonTest, InstantiateOnce)
{
    Singleton<MyClass> s;

    EXPECT_NO_THROW(s.instantiate());
    EXPECT_NE(s.instance(), nullptr);
}

TEST(SingletonTest, DoubleInstantiateThrows)
{
    Singleton<MyClass> s;
    EXPECT_THROW(s.instantiate(), std::runtime_error);
    s.reset();
    EXPECT_THROW(s.instance(), std::runtime_error);
}

TEST(SingletonTest, AccessInstance)
{
    Singleton<MyClass> s;
    s.instantiate();

    MyClass* ptr = s.instance();
    EXPECT_NE(ptr, nullptr);

    int oldValue = ptr->value;
    ptr->someMethod();
    EXPECT_EQ(ptr->value, oldValue + 1);
    s.reset();
}

TEST(SingletonTest, InstantiateWithArguments)
{
    Singleton<MyClass> s;
    EXPECT_NO_THROW(s.instantiate(42, "hello"));

    MyClass* ptr = s.instance();
    EXPECT_EQ(ptr->value, 42);
    EXPECT_EQ(ptr->name, "hello");
}
