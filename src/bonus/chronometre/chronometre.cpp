#include "chronometre.hpp"
void Chronometre::error()
{
    throw std::runtime_error("Chronometre::getTime(): start() and end() must be called in pairs");
}

void Chronometre::createTimestamp()
{
    _now = std::chrono::system_clock::now();
    _timestamps.push_back(
        std::chrono::duration_cast<std::chrono::nanoseconds>(_now.time_since_epoch()).count());
}
void Chronometre::start()
{
    createTimestamp();
}
void Chronometre::end()
{
    if (_timestamps.size() == 0)
        error();
    createTimestamp();
}

void Chronometre::popLastChrono()
{
    if (_timestamps.size() > 1)
    {
        _timestamps.pop_back();
        _timestamps.pop_back();
    }
}

double Chronometre::getTimeNanoseconds()
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        error();
    return _timestamps.back() - _timestamps[_timestamps.size() - 2];
}

double Chronometre::getTimeMicroseconds()
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        error();

    return (_timestamps.back() - _timestamps[_timestamps.size() - 2]) / 1000.0;
}

double Chronometre::getTimeMilliseconds()
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        error();

    return (_timestamps.back() - _timestamps[_timestamps.size() - 2]) / 1000000.0;
}

double Chronometre::getTimeSeconds()
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        error();

    return (_timestamps.back() - _timestamps[_timestamps.size() - 2]) / 1000000000.0;
}

std::string Chronometre::getTimeString()
{
    if (_timestamps.size() == 0 || _timestamps.size() % 2 != 0)
        error();

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