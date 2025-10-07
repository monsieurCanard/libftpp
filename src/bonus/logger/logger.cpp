#include "logger.hpp"

Logger::Logger()
{
    setOutputFile("");
}

Logger::Logger(const std::string& outputfile, LogLevel level) : _currentLevel(level)
{
    setOutputFile(outputfile);
}

const std::string Logger::getOutputPathFile() const
{
    return _outputFilePath + _logFilename + ".log";
}

Logger::~Logger()
{
    if (_logFile.is_open())
        _logFile.close();
    _writer << std::flush;
}

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level)
{
    _currentLevel = level;
}

void Logger::setOutputFile(const std::string& filename)
{
    char buffer[1024];
    if (!getcwd(buffer, 1024))
        throw std::runtime_error("Current path for Output file could not be determined try to "
                                 "increase filename buffer size" +
                                 std::to_string(errno));
    if (filename.empty())
        _logFilename = getCurrentTime();
    else
        _logFilename = filename;

    if (_outputFilePath.empty())
        _outputFilePath = std::string(buffer) + "/logs/";

    std::string dirPath = _outputFilePath;
    if (!dirPath.empty() && dirPath.back() == '/')
        dirPath.pop_back();

    struct stat st;
    if (stat(dirPath.c_str(), &st) != 0)
    {
        if (mkdir(dirPath.c_str(), 0755) != 0 && errno != EEXIST)
        {
            throw std::runtime_error(std::string("Could not create log directory: ") +
                                     std::to_string(errno));
        }
    }
    else if (!S_ISDIR(st.st_mode))
    {
        throw std::runtime_error("Log path exists but is not a directory");
    }

    if (_logFile.is_open())
        _logFile.close();

    // std::ios::out -> cree si il existe pas + ecriture
    // std::ios::app -> O_APPEND se place et ecris a la fin du fichier de destination
    _logFile.open(_outputFilePath + _logFilename + ".log", std::ios::out | std::ios::app);
    if (!_logFile.is_open())
        throw std::runtime_error("Could not open log file: " + _logFilename + ".log");
}

std::string Logger::getCurrentTime() const
{
    auto        now   = std::chrono::system_clock::now();
    std::time_t clock = std::chrono::system_clock::to_time_t(now);
    std::tm     localTime;

    localtime_r(&clock, &localTime); // Linux / Mac
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d | %H:%M:%S");
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level) const
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message)
{
    if (level < _currentLevel)
        return;
    std::lock_guard<std::mutex> lock(_fileMutex);
    _logFile << "[" << logLevelToString(level) << "]" << "[" << getCurrentTime() << "] " << message
             << std::endl;
}

void Logger::logConsole(LogLevel level, const std::string& message)
{
    if (level < _currentLevel)
        return;
    _writer << "[" << logLevelToString(level) << "]" << "[" << getCurrentTime() << "] " << message
            << std::endl;
}