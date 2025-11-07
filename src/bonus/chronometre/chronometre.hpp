#ifndef CHRONOMETRE_HPP
#define CHRONOMETRE_HPP

#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Chronometre class for measuring elapsed time between start and end timestamps.
 * It provides methods to retrieve the elapsed time in various units (nanoseconds, microseconds,
 * milliseconds, seconds) and as a formatted string.
 */
class Chronometre
{
private:
    std::vector<double>                        _timestamps;
    std::chrono::_V2::system_clock::time_point _now;

    void error();
    void createTimestamp();

public:
    void start();
    void end();

    void popLastChrono();

    double      getTimeNanoseconds();
    double      getTimeMicroseconds();
    double      getTimeMilliseconds();
    double      getTimeSeconds();
    std::string getTimeString();
};

#endif