#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
#define RANDOM_2D_COORDINATE_GENERATOR_HPP

#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <functional>
/**
 * @brief 2D Coordinate-Based Random Number Generator
 * @note Generates consistent pseudo-random numbers based on 2D coordinates and a seed.
 *
 * @code
 * Random2DCoordinateGenerator rng1; // Seed based on current time
 * Random2DCoordinateGenerator rng2(42); // Seed based on user-defined value
 *
 * long long randomValue1 = rng1(10, 20); // Get random value for coordinates (10, 20)
 * long long randomValue2 = rng2(10, 20); // Get random value for coordinates (10, 20)
 * @endcode
 */
class Random2DCoordinateGenerator
{
private:
    long long _seed;

public:
    Random2DCoordinateGenerator();
    Random2DCoordinateGenerator(long long seed);

    long long seed();
    long long operator()(const long long& x, const long long& y);
};
#endif