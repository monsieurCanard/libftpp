#include <gtest/gtest.h>

#include "data_buffer.hpp"

// --- Test int ---
TEST(DataBufferTest, IntSerialization)
{
    DataBuffer buf;
    int        x = 42, y = 0;

    buf << x; // sérialisation
    buf >> y; // désérialisation

    EXPECT_EQ(x, y);
}

// --- Test double ---
TEST(DataBufferTest, DoubleSerialization)
{
    DataBuffer buf;
    double     a = 3.14159, b = 0.0;

    buf << a;
    buf >> b;

    EXPECT_DOUBLE_EQ(a, b);
}

// --- Test char ---
TEST(DataBufferTest, CharSerialization)
{
    DataBuffer buf;
    char       c1 = 'A', c2 = '\0';

    buf << c1;
    buf >> c2;

    EXPECT_EQ(c1, c2);
}

// --- Test float ---
TEST(DataBufferTest, FloatSerialization)
{
    DataBuffer buf;
    float      f1 = 1.23f, f2 = 0.0f;

    buf << f1;
    buf >> f2;

    EXPECT_FLOAT_EQ(f1, f2);
}

// --- Test multiple values ---
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

// --- Test buffer remains intact après plusieurs sérialisations ---
TEST(DataBufferTest, MultipleWrites)
{
    DataBuffer buf;
    int        a = 1, b = 2, c = 0;

    buf << a << b;
    buf >> c;

    // Ici on ne lit que le premier int, il doit être égal à a
    EXPECT_EQ(c, a);
}

// --- Test empty buffer (valeur par défaut reste inchangée) ---
TEST(DataBufferTest, EmptyBuffer)
{
    DataBuffer buf;
    int        x = 123;
    int        y = x;

    EXPECT_THROW(buf >> y, std::runtime_error);
}

// // --- Main ---
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
