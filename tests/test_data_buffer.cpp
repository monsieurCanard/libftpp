#include <gtest/gtest.h>

#include "libftpp.hpp"

TEST(DataBufferTest, IntSerialization)
{
    DataBuffer buf;
    int        x = 42, y = 0;

    buf << x;
    buf >> y;

    EXPECT_EQ(x, y);
}

TEST(DataBufferTest, StringSerialization)
{
    DataBuffer  buf;
    std::string x = "42", y = "";

    buf << x;
    buf >> y;

    EXPECT_EQ(x, y);
}

TEST(DataBufferTest, DoubleSerialization)
{
    DataBuffer buf;
    double     a = 3.14159, b = 0.0;

    buf << a;
    buf >> b;

    EXPECT_DOUBLE_EQ(a, b);
}

TEST(DataBufferTest, CharSerialization)
{
    DataBuffer buf;
    char       c1 = 'A', c2 = '\0';

    buf << c1;
    buf >> c2;

    EXPECT_EQ(c1, c2);
}

TEST(DataBufferTest, FloatSerialization)
{
    DataBuffer buf;
    float      f1 = 1.23f, f2 = 0.0f;

    buf << f1;
    buf >> f2;

    EXPECT_FLOAT_EQ(f1, f2);
}

TEST(DataBufferTest, MultipleValuesSerialization)
{
    DataBuffer buf;
    int        i = 10, j = 0;
    double     d = 2.718, e = 0.0;

    buf << i << d;
    buf >> j >> e;

    EXPECT_EQ(i, j);
    EXPECT_DOUBLE_EQ(d, e);
}

TEST(DataBufferTest, EmptyBuffer)
{
    DataBuffer buf;
    int        x = 123;
    int        y = x;

    EXPECT_THROW(buf >> y, std::out_of_range);
}
