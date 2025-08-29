#include <gtest/gtest.h>

#include "data_buffer.hpp"

TEST(DataBufferTest, IntSerialization)
{
    DataBuffer buf;

    int x = 42;
    int y = 0;

    buf << x; // sérialisation
    buf >> y; // désérialisation

    EXPECT_EQ(x, y); // vérifie que la valeur lue est égale à la valeur écrite
}

TEST(DataBufferTest, DoubleSerialization)
{
    DataBuffer buf;

    double a = 3.14;
    double b = 0.0;

    buf << a;
    buf >> b;

    EXPECT_DOUBLE_EQ(a, b);
}
