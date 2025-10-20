#include <gtest/gtest.h>

#include <stdexcept>

#include "libftpp.hpp"

TEST(MessageTest, TypeIsCorrect)
{
    Message msg(42);
    EXPECT_EQ(msg.type(), 42);
}

TEST(MessageTest, WriteAndReadInt)
{
    Message msg(1);
    int     valueOut = 0;
    int     valueIn  = 12345;

    msg << valueIn;
    msg.reset();

    EXPECT_NO_THROW(msg >> valueOut);
}

TEST(MessageTest, WriteAndReadFloat)
{
    Message msg(2);
    float   valueOut = 3.14f;
    float   valueIn  = 3.14f;

    msg << valueIn;
    msg >> valueOut;

    EXPECT_FLOAT_EQ(valueOut, valueIn);
}

TEST(MessageTest, WriteAndReadMultipleTypes)
{
    Message msg(3);
    int     iIn = 42, iOut = 42;
    double  dIn = 6.28, dOut = 6.28;

    msg << iIn << dIn;
    msg >> iOut >> dOut;

    EXPECT_EQ(iOut, iIn);
    EXPECT_DOUBLE_EQ(dOut, dIn);
}

TEST(MessageTest, OutOfBoundsThrows)
{
    Message msg(4);
    int     big = 999;
    // Simule un mauvais _cursor en forçant une lecture hors buffer
    EXPECT_THROW(
        {
            msg.reset();
            msg >> big;
        },
        std::out_of_range);
}

TEST(MessageTest, ResetWorks)
{
    Message msg(2);
    int     valIn = 2025;
    int     valOut;

    msg << valIn;
    msg >> valOut;

    EXPECT_EQ(valOut, valIn);

    msg.reset(); // On repart au début
    valOut = 0;
    msg << valOut;
    msg >> valOut;
    EXPECT_EQ(valOut, valIn); // relu encore une fois
}

class MessageComplexTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        message = std::make_unique<Message>(10);
    }

    std::unique_ptr<Message> message;
};

TEST_F(MessageComplexTest, MessageWithMultipleDataTypes)
{
    int         testInt    = 42;
    double      testDouble = 3.14159;
    std::string testString = "Test String";

    *message << testInt << testDouble;
    *message << testString.length();
    for (char c : testString)
    {
        *message << c;
    }

    int         retrievedInt;
    double      retrievedDouble;
    std::string retrievedString;

    *message >> retrievedInt >> retrievedDouble;
    size_t strLength;
    *message >> strLength;

    std::string text;
    text.reserve(strLength);
    for (size_t i = 0; i < strLength; ++i)
    {
        char c;
        *message >> c;
        text.push_back(c);
    }
    retrievedString = text;

    EXPECT_EQ(retrievedInt, testInt);
    EXPECT_DOUBLE_EQ(retrievedDouble, testDouble);
    EXPECT_EQ(retrievedString, testString);
}

TEST_F(MessageComplexTest, MessageLargeString)
{
    std::string largeString(1000, 'A');
    size_t      length = largeString.length();
    *message << length;
    for (char c : largeString)
    {
        *message << c;
    }

    std::string retrievedString;
    size_t      strLength;
    *message >> strLength;
    retrievedString.reserve(strLength);

    for (size_t i = 0; i < strLength; ++i)
    {
        char c;
        *message >> c;
        retrievedString.push_back(c);
    }

    EXPECT_EQ(retrievedString, largeString);
}
