#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
#define RANDOM_2D_COORDINATE_GENERATOR_HPP

#include <unistd.h>

#include <chrono>
#include <functional>

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