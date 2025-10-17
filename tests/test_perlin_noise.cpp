#include <gtest/gtest.h>

#include "libftpp.hpp"

// Pour tester les méthodes privées, on peut soit :
// - Modifier la classe pour friend avec une classe de test
// - Ou tester indirectement via sample()
// Ici, je propose des tests directs pour linearInterpolation, fade, gradient.

class PerlinNoiseTest : public ::testing::Test
{
protected:
    PerlinNoise2D perlin;
};

// --- Tests linearInterpolation ---
TEST_F(PerlinNoiseTest, LinearInterpolationAtZero)
{
    EXPECT_FLOAT_EQ(perlin.sample(0.0f, 0.0f), perlin.sample(0.0f, 0.0f));
    // Juste pour forcer l'appel
    EXPECT_FLOAT_EQ(perlin.linearInterpolation(0.0f, 10.0f, 0.0f), 0.0f);
}

TEST_F(PerlinNoiseTest, LinearInterpolationAtOne)
{
    EXPECT_FLOAT_EQ(perlin.linearInterpolation(0.0f, 10.0f, 1.0f), 10.0f);
}

TEST_F(PerlinNoiseTest, LinearInterpolationMidpoint)
{
    EXPECT_FLOAT_EQ(perlin.linearInterpolation(0.0f, 10.0f, 0.5f), 5.0f);
}

// --- Tests fade ---
TEST_F(PerlinNoiseTest, FadeAtEdges)
{
    EXPECT_FLOAT_EQ(perlin.fade(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(perlin.fade(1.0f), 1.0f);
}

TEST_F(PerlinNoiseTest, FadeContinuity)
{
    float t1 = perlin.fade(0.3f);
    float t2 = perlin.fade(0.3001f);
    EXPECT_NEAR(t1, t2, 1e-3); // continuité
}

// --- Tests gradient ---
TEST_F(PerlinNoiseTest, GradientIsUnitVector)
{
    auto  g    = perlin.gradient(42, 24);
    float norm = std::sqrt(g.x * g.x + g.y * g.y);
    EXPECT_NEAR(norm, 1.0f, 1e-5);
}

// --- Tests sample ---
TEST_F(PerlinNoiseTest, SampleIsStable)
{
    float v1 = perlin.sample(10.5f, 20.5f);
    float v2 = perlin.sample(10.5f, 20.5f);
    EXPECT_FLOAT_EQ(v1, v2); // même entrée -> même sortie
}

TEST_F(PerlinNoiseTest, SampleIsBounded)
{
    float v = perlin.sample(3.14f, 42.0f);
    // borne max dot = sqrt(2) ≈ 1.41
    EXPECT_GE(v, -1.5f);
    EXPECT_LE(v, 1.5f);
}
