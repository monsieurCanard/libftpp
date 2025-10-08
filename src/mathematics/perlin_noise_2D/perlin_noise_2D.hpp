#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <cmath>

#include "../IVector2/ivector2.hpp"
#include "../random_2D_coordinate_generator/random_2D_coordinate_generator.hpp"
class PerlinNoise2D
{
private:
    int _seedGlobal = 29874789; // Chiffre choisie aleatoirement

public:
    float           linearInterpolation(float a, float b, float t);
    IVector2<float> gradient(const int& i, const int& j);
    float           fade(const float& t);
    float           sample(const float& x, const float& y);
};

#endif
