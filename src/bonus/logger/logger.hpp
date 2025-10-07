#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <string>

#include "../../thread/thread_safe_iostream/thread_safe_iostream.hpp"

enum class LogLevel
{
    DEBUG   = 0,
    INFO    = 1,
    WARNING = 2,
    ERROR   = 3
};
/**
 * @brief Logger class for logging messages to console and file with different log levels.
 * This class is implemented as a singleton to ensure a single instance throughout the application.
 * @note Thread safety is ensured for console output and file output
 * @note You must call setOutputFile before logging to file, if you didn't provide a filename a
 * timestamped one will be generated
 */
class Logger
{
private:
    LogLevel    _currentLevel = LogLevel::DEBUG;
    std::string _logFilename;
    std::string _outputFilePath;

    std::mutex         _fileMutex;
    ThreadSafeIOStream _writer;
    std::ofstream      _logFile;

    std::string getCurrentTime() const;
    std::string logLevelToString(LogLevel level) const;

public:
    Logger();
    Logger(const std::string& outputfile, LogLevel level = LogLevel::DEBUG);
    ~Logger();

    static Logger& instance();

    void setLogLevel(LogLevel level);
    void setOutputFile(const std::string& filename);

    const std::string getOutputPathFile() const;

    void log(LogLevel level, const std::string& message);
    void logConsole(LogLevel level, const std::string& message);
};

#endif