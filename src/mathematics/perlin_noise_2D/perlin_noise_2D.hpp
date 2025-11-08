#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <cmath>

#include "../IVector2/ivector2.hpp"
#include "../random_2D_coordinate_generator/random_2D_coordinate_generator.hpp"
/**
 * @brief 2D Perlin Noise Generator
 *
 * @code
 * PerlinNoise2D perlin;
 * 
 * float         value = perlin.sample(1.5f, 2.3f);
 * 
 * @endcode
 *
 * @note The sample function returns values in the range [-1, 1].
 */
class PerlinNoise2D
{
private:
    Random2DCoordinateGenerator generator;
    float                       linearInterpolation(float a, float b, float t);
    IVector2<float>             gradient(const int& i, const int& j);
    float                       fade(const float& t);

public:
    float sample(const float& x, const float& y);
};

#endif
