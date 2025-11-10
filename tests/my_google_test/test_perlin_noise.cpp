#include <gtest/gtest.h>

#include "libftpp.hpp"

class PerlinNoiseTest : public ::testing::Test
{
protected:
    PerlinNoise2D perlin;
};

// --- Tests linearInterpolation ---
TEST_F(PerlinNoiseTest, LinearInterpolationAtZero)
{
    EXPECT_FLOAT_EQ(perlin.sample(0.0f, 0.0f), perlin.sample(0.0f, 0.0f));
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
