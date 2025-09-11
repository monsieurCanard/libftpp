#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <filesystem>
#include <fstream>
#include <string>

#include "../../thread/thread_safe_iostream/thread_safe_iostream.hpp"

enum class logLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger
{
private:
    logLevel    _currentLevel = logLevel::DEBUG;
    std::string _logFilenamePrefix;

    ThreadSafeIOStream _writer;
    std::ofstream      logFile;

public:
    bool _outputConsole = false;

    static Logger& instance()
    {
        static Logger instance;
        return instance;
    }

    void setLogLevel(logLevel level)
    {
        _currentLevel = level;
    }

    void setOuputFile(const std::string& filename)
    {
        auto cwd = std::filesystem::current_path();
    }

    void log(logLevel level, const std::string& message) {}
};
#endif