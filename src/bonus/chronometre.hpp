#ifndef CHRONOMETRE_HPP
#define CHRONOMETRE_HPP

#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

class Chronometre
{
private:
    std::vector<double>                        timestamps;
    std::chrono::_V2::system_clock::time_point now;

public:
    void error()
    {
        throw std::runtime_error(
            "Chronometre::getTime(): start() and end() must be called in pairs");
    }
    void start()
    {
        now = std::chrono::system_clock::now();
        timestamps.push_back(
            std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count());
    }
    void end()
    {
        start();
    }

    void popLastChrono()
    {
        if (timestamps.size() > 1)
        {
            timestamps.pop_back();
            timestamps.pop_back();
        }
    }

    double getTimeMicroseconds()
    {
        if (timestamps.size() == 0 || timestamps.size() % 2 != 0)
            error();
        return timestamps.back() - timestamps[timestamps.size() - 2];
    }

    double getTimeMilliseconds()
    {
        if (timestamps.size() == 0 || timestamps.size() % 2 != 0)
            error();

        return (timestamps.back() - timestamps[timestamps.size() - 2]) / 1000.0;
    }

    double getTimeSeconds()
    {
        if (timestamps.size() == 0 || timestamps.size() % 2 != 0)
            error();

        return (timestamps.back() - timestamps[timestamps.size() - 2]) / 1000000.0;
    }

    std::string getTimeString()
    {
        if (timestamps.size() == 0 || timestamps.size() % 2 != 0)
            error();

        double microseconds = timestamps.back() - timestamps[timestamps.size() - 2];
        if (microseconds < 1000)
            return std::to_string(microseconds) + " µs";
        else if (microseconds < 1000000)
            return std::to_string(microseconds / 1000.0) + " ms";
        else
            return std::to_string(microseconds / 1000000.0) + " s";
    }
};
#endif