#include "chronometre.hpp"

Chronometre::~Chronometre()
{
    _timestamps.clear();
}

void Chronometre::createTimestamp()
{
    _now = std::chrono::system_clock::now();
    _timestamps.push_back(
        std::chrono::duration_cast<std::chrono::nanoseconds>(_now.time_since_epoch()).count());
}
/**
 * @brief Start the chronometre.
 */
void Chronometre::start()
{
    createTimestamp();
}

/**
 * @brief End the chronometre.
 */
void Chronometre::end()
{
    if (_timestamps.size() == 0)
        throw std::logic_error("Chronometre::end(): start() must be called before end()");

    createTimestamp();
}

/**
 * @brief Remove the last recorded time interval (start and end timestamps).
 */
void Chronometre::popLastChrono()
{
    if (_timestamps.size() > 1)
    {
        _timestamps.pop_back();
        _timestamps.pop_back();
    }
}

/**
 * @brief Get the elapsed time in nanoseconds between the last start and end timestamps.
 * @return Elapsed time in nanoseconds.
 */
double Chronometre::getTimeNanoseconds() const
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        throw std::logic_error("Chronometre::getTime(): start() and end() must be called in pairs");

    return _timestamps.back() - _timestamps[_timestamps.size() - 2];
}

/**
 * @brief Get the elapsed time in microseconds between the last start and end timestamps.
 * @return Elapsed time in microseconds.
 */
double Chronometre::getTimeMicroseconds() const
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        throw std::logic_error("Chronometre::getTime(): start() and end() must be called in pairs");

    return (_timestamps.back() - _timestamps[_timestamps.size() - 2]) / 1000.0;
}

/**
 * @brief Get the elapsed time in milliseconds between the last start and end timestamps.
 * @return Elapsed time in milliseconds.
 */
double Chronometre::getTimeMilliseconds() const
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        throw std::logic_error("Chronometre::getTime(): start() and end() must be called in pairs");

    return (_timestamps.back() - _timestamps[_timestamps.size() - 2]) / 1000000.0;
}

/**
 * @brief Get the elapsed time in seconds between the last start and end timestamps.
 * @return Elapsed time in seconds.
 */
double Chronometre::getTimeSeconds() const
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        throw std::logic_error("Chronometre::getTime(): start() and end() must be called in pairs");

    return (_timestamps.back() - _timestamps[_timestamps.size() - 2]) / 1000000000.0;
}

/**
 * @brief Get the elapsed time as a formatted string with appropriate units.
 * @warning You cannot choose the unit, it is determined automatically based on the elapsed time.
 * @return Formatted string representing the elapsed time with units (ns, µs, ms, s).
 */
std::string Chronometre::getTimeString() const
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        throw std::logic_error("Chronometre::getTime(): start() and end() must be called in pairs");

    double nanoseconds = _timestamps.back() - _timestamps[_timestamps.size() - 2];
    if (nanoseconds < 1000)
        return std::to_string(nanoseconds) + " ns";
    else if (nanoseconds < 1000000)
        return std::to_string(nanoseconds / 1000.0) + " µs";
    else if (nanoseconds < 1000000000)
        return std::to_string(nanoseconds / 1000000.0) + " ms";
    else
        return std::to_string(nanoseconds / 1000000000.0) + " s";
}