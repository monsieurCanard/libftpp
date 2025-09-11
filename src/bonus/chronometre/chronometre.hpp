#ifndef CHRONOMETRE_HPP
#define CHRONOMETRE_HPP

#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

class Chronometre
{
private:
    std::vector<double>                        _timestamps;
    std::chrono::_V2::system_clock::time_point _now;
    void                                       error();
    void                                       createTimestamp();

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