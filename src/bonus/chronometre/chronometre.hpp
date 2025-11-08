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
 *
 *
 * @code
 * Chronometre chrono;
 * chrono.start();
 * chrono.end();
 * double elapsed = chrono.getTimeMilliseconds();
 * std::cout << "Elapsed time: " << elapsed << " ms" << std::endl;
 * chrono.popLastChrono(); // Remove the last recorded time interval
 * @endcode
 *
 * @warning start() and end() must be called in pairs before retrieving the elapsed time.
 */
class Chronometre
{
private:
    std::vector<double>                        _timestamps;
    std::chrono::_V2::system_clock::time_point _now;

    void createTimestamp();

public:
    Chronometre() = default;
    ~Chronometre();

    void start();
    void end();

    void popLastChrono();

    double      getTimeNanoseconds() const;
    double      getTimeMicroseconds() const;
    double      getTimeMilliseconds() const;
    double      getTimeSeconds() const;
    std::string getTimeString() const;
};

#endif