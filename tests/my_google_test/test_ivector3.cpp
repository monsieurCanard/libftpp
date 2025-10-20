#include <cmath>
#include <stdexcept>

#include "gtest/gtest.h"
#include "libftpp.hpp"

using Vec3f     = IVector3<float>;
const float EPS = 1e-6f;

// --- Opérateurs +, -, *, / ---
TEST(Vector3DTest, Operators)
{
    Vec3f a(3, 4, 5);
    Vec3f b(1, 2, 3);

    // Addition
    Vec3f add = a + b;
    EXPECT_FLOAT_EQ(add.x, 4);
    EXPECT_FLOAT_EQ(add.y, 6);
    EXPECT_FLOAT_EQ(add.z, 8);

    // Soustraction
    Vec3f sub = a - b;
    EXPECT_FLOAT_EQ(sub.x, 2);
    EXPECT_FLOAT_EQ(sub.y, 2);
    EXPECT_FLOAT_EQ(sub.z, 2);

    // Multiplication terme à terme
    Vec3f mul = a * b;
    EXPECT_FLOAT_EQ(mul.x, 3);
    EXPECT_FLOAT_EQ(mul.y, 8);
    EXPECT_FLOAT_EQ(mul.z, 15);

    // Division terme à terme
    Vec3f c(6, 8, 10);
    Vec3f d(2, 4, 5);
    Vec3f div = c / d;
    EXPECT_FLOAT_EQ(div.x, 3);
    EXPECT_FLOAT_EQ(div.y, 2);
    EXPECT_FLOAT_EQ(div.z, 2);

    // Division par zéro
    Vec3f zero(0, 0, 0);
    EXPECT_THROW(c / zero, std::invalid_argument);
}

// --- Opérateurs == et != ---
TEST(Vector3DTest, Equality)
{
    Vec3f a(3, 4, 5);
    Vec3f b(3, 4, 5);
    Vec3f c(4, 3, 5);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a != b);
}

// --- Opérateurs += et -= ---
TEST(Vector3DTest, CompoundAssignment)
{
    Vec3f a(1, 2, 3);
    Vec3f b(3, 4, 5);

    a += b;
    EXPECT_FLOAT_EQ(a.x, 4);
    EXPECT_FLOAT_EQ(a.y, 6);
    EXPECT_FLOAT_EQ(a.z, 8);

    a -= b;
    EXPECT_FLOAT_EQ(a.x, 1);
    EXPECT_FLOAT_EQ(a.y, 2);
    EXPECT_FLOAT_EQ(a.z, 3);
}

// --- Length et normalize ---
TEST(Vector3DTest, LengthAndNormalize)
{
    Vec3f a(3, 4, 12);

    EXPECT_FLOAT_EQ(a.length(), 13.0f);

    Vec3f n = a.normalize();
    EXPECT_NEAR(n.length(), 1.0f, EPS);
    EXPECT_NEAR(n.x, 3.0f / 13.0f, EPS);
    EXPECT_NEAR(n.y, 4.0f / 13.0f, EPS);
    EXPECT_NEAR(n.z, 12.0f / 13.0f, EPS);

    Vec3f zero(0, 0, 0);
    EXPECT_THROW(zero.normalize(), std::invalid_argument);
}

// --- Dot product ---
TEST(Vector3DTest, Dot)
{
    Vec3f a(3, 4, 5);
    Vec3f b(1, 2, 3);

    // Dot avec lui-même
    EXPECT_FLOAT_EQ(a.dot(), 3 * 3 + 4 * 4 + 5 * 5); // 50

    // Dot avec autre vecteur
    EXPECT_FLOAT_EQ(a.dot(b), 3 * 1 + 4 * 2 + 5 * 3); // 26
}

TEST(Vector3DTest, Cross)
{
    Vec3f a(1, 0, 0);
    Vec3f b(0, 1, 0);

    // Cross product 3D
    Vec3f c = a.cross(b); // devrait renvoyer (0,0,1)
    EXPECT_FLOAT_EQ(c.x, 0);
    EXPECT_FLOAT_EQ(c.y, 0);
    EXPECT_FLOAT_EQ(c.z, 1);

    // Test avec d'autres vecteurs
    Vec3f d(2, 3, 4);
    Vec3f e(5, 6, 7);
    Vec3f f = d.cross(e);

    EXPECT_FLOAT_EQ(f.x, 3 * 7 - 4 * 6); // -3
    EXPECT_FLOAT_EQ(f.y, 4 * 5 - 2 * 7); // 6
    EXPECT_FLOAT_EQ(f.z, 2 * 6 - 3 * 5); // -3
}
