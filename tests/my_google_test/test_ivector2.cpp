#include <cmath>
#include <stdexcept>

#include "gtest/gtest.h"
#include "libftpp.hpp"

using Vec2f = IVector2<float>;

const float EPS = 1e-6f;

// --- Opérateurs +, -, *, / ---
TEST(Vector2DTest, Operators)
{
    Vec2f a(3, 4);
    Vec2f b(1, 2);

    // Addition
    Vec2f add = a + b;
    EXPECT_FLOAT_EQ(add.x, 4);
    EXPECT_FLOAT_EQ(add.y, 6);

    // Soustraction
    Vec2f sub = a - b;
    EXPECT_FLOAT_EQ(sub.x, 2);
    EXPECT_FLOAT_EQ(sub.y, 2);

    // Multiplication terme à terme
    Vec2f mul = a * b;
    EXPECT_FLOAT_EQ(mul.x, 3);
    EXPECT_FLOAT_EQ(mul.y, 8);

    // Division terme à terme
    Vec2f c(6, 8);
    Vec2f d(2, 4);
    Vec2f div = c / d;
    EXPECT_FLOAT_EQ(div.x, 3);
    EXPECT_FLOAT_EQ(div.y, 2);

    // Division par zéro
    Vec2f zero(0, 0);
    EXPECT_THROW(c / zero, std::invalid_argument);
}

// --- Opérateurs == et != ---
TEST(Vector2DTest, Equality)
{
    Vec2f a(3, 4);
    Vec2f b(3, 4);
    Vec2f c(4, 3);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a != b);
}

// --- Opérateurs += et -= ---
TEST(Vector2DTest, CompoundAssignment)
{
    Vec2f a(1, 2);
    Vec2f b(3, 4);

    a += b;
    EXPECT_FLOAT_EQ(a.x, 4);
    EXPECT_FLOAT_EQ(a.y, 6);

    a -= b;
    EXPECT_FLOAT_EQ(a.x, 1);
    EXPECT_FLOAT_EQ(a.y, 2);
}

// --- Length et normalize ---
TEST(Vector2DTest, LengthAndNormalize)
{
    Vec2f a(3, 4);

    EXPECT_FLOAT_EQ(a.length(), 5.0f);

    Vec2f n = a.normalize();
    EXPECT_NEAR(n.length(), 1.0f, EPS);
    EXPECT_NEAR(n.x, 0.6f, EPS);
    EXPECT_NEAR(n.y, 0.8f, EPS);

    Vec2f zero(0, 0);
    EXPECT_THROW(zero.normalize(), std::invalid_argument);
}

// --- Dot product ---
TEST(Vector2DTest, Dot)
{
    Vec2f a(3, 4);
    Vec2f b(1, 2);

    // Dot avec lui-même
    EXPECT_FLOAT_EQ(a.dot(), 25.0f);

    // Dot avec autre vecteur
    EXPECT_FLOAT_EQ(a.dot(b), 3 * 1 + 4 * 2); // = 11
}

TEST(Vector2DTest, Cross)
{
    Vec2f a(3, 4);
    Vec2f b(1, 2);

    // Cross classique avec un autre vecteur (2D scalar cross)
    EXPECT_FLOAT_EQ(a.cross(b), 3 * 2 - 4 * 1); // = 2

    // Cross avec un seul vecteur → vecteur perpendiculaire (-y, x)
    Vec2f perp = a.cross();
    EXPECT_FLOAT_EQ(perp.x, -4); // -y
    EXPECT_FLOAT_EQ(perp.y, 3);  // x
}
