#include "random_2D_coordinate_generator.hpp"

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long seed) : _seed(seed) {}

Random2DCoordinateGenerator::Random2DCoordinateGenerator()
{
    // Seed based on time in microseconds since epoch
    const auto now = std::chrono::system_clock::now();
    _seed = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

    // Mix with PID to give to each process a different seed
    _seed ^= getpid();

    int x;
    // Generated unique hash based on int x address in memory
    _seed ^= std::hash<int*>{}(&x);
}

long long Random2DCoordinateGenerator::seed()
{
    return _seed;
}

long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y)
{
    return std::hash<long long>{}(_seed ^ (x * 7 - y * 13));
}
