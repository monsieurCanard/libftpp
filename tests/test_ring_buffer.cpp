#include <gtest/gtest.h>

#include <vector>

#include "libftpp.hpp"

class RingBufferTest : public ::testing::Test
{
protected:
    RingBuffer buffer;

    void SetUp() override
    {
        buffer = RingBuffer(8); // buffer de 8 octets pour les tests
    }
};

// Test push/pop d'un octet simple
TEST_F(RingBufferTest, PushPopSingleByte)
{
    buffer.push(42);
    EXPECT_EQ(buffer.size(), 1u);
    unsigned char val = buffer.pop();
    EXPECT_EQ(val, 42);
    EXPECT_TRUE(buffer.isEmpty());
}

// Test push/pop multiples octets
TEST_F(RingBufferTest, PushPopMultipleBytes)
{
    std::vector<unsigned char> data = {1, 2, 3, 4};
    buffer.push(data);
    EXPECT_EQ(buffer.size(), 4u);

    std::vector<unsigned char> result = buffer.pop(4);
    EXPECT_EQ(result, data);
    EXPECT_TRUE(buffer.isEmpty());
}

// Test overflow (push plus grand que le buffer)
TEST_F(RingBufferTest, OverflowThrows)
{
    std::vector<unsigned char> bigData(9, 0xFF); // 9 > 8
    EXPECT_THROW(buffer.push(bigData), std::runtime_error);
}

// Test wrap-around
TEST_F(RingBufferTest, WrapAround)
{
    buffer.push(std::vector<unsigned char>{1, 2, 3, 4, 5});
    EXPECT_EQ(buffer.pop(), 1);
    EXPECT_EQ(buffer.pop(), 2);
    buffer.push(std::vector<unsigned char>{6, 7, 8});

    std::vector<unsigned char> expected = {3, 4, 5, 6, 7, 8};
    std::vector<unsigned char> result   = buffer.pop(6);
    EXPECT_EQ(result, expected);
}

// Test peek sans avancer le tail
TEST_F(RingBufferTest, Peek)
{
    buffer.push(std::vector<unsigned char>{10, 20, 30});
    std::vector<unsigned char> peeked = buffer.peek(2);
    EXPECT_EQ(peeked.size(), 2u);
    EXPECT_EQ(peeked[0], 10);
    EXPECT_EQ(peeked[1], 20);
    EXPECT_EQ(buffer.size(), 3u); // taille inchangée
}

// Test vider le buffer
TEST_F(RingBufferTest, Clear)
{
    buffer.push(std::vector<unsigned char>{1, 2, 3});
    buffer.clear();
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_EQ(buffer.size(), 0u);
}

// Test exception quand pop sur buffer vide
TEST_F(RingBufferTest, PopEmptyThrows)
{
    EXPECT_THROW(buffer.pop(), std::runtime_error);
    EXPECT_THROW(buffer.pop(1), std::runtime_error);
}
