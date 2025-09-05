#ifndef THREAD_SAFE_IOSTREAM
#define THREAD_SAFE_IOSTREAM

#include <iostream>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>

class ThreadSafeIOStream
{
private:
    std::string _prefix;
    std::mutex  _mutex;
    bool        _needPrefix = true;

public:
    template <typename T>
    ThreadSafeIOStream& operator>>(T& value)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        while (1)
        {
            std::cin >> value;
            if (std::cin.fail())
            {
                std::cout << "Invalid input !" << std::endl;
                std::cin.clear();

                // Vide completement le buffer (limits.h)
                std::cin.ignore(INT32_MAX, '\n');
                continue;
            }
            break;
        }
        return *this;
    }

    ThreadSafeIOStream& operator<<(std::ostream& (*funct)(std::ostream&));

    template <typename T>
    ThreadSafeIOStream& operator<<(const T& value)
    {
        std::ostringstream ss;
        ss << value;
        std::string            line   = ss.str();
        std::string::size_type pos    = 0;
        std::string::size_type cursor = 0;

        std::lock_guard<std::mutex> lock(_mutex);
        if (_needPrefix)
        {
            std::cout << _prefix;
            _needPrefix = false;
        }

        while ((pos = line.find('\n', cursor)) != std::string::npos)
        {
            if (_needPrefix)
            {
                std::cout << _prefix;
                _needPrefix = false;
            }

            std::cout << line.substr(cursor, pos - cursor) << '\n';
            cursor      = pos + 1;
            _needPrefix = true;
        }

        std::cout << line.substr(cursor);
        return *this;
    }

    void setPrefix(const std::string& prefix);

    template <typename T>
    void prompt(const std::string& question, T& dest)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            std::cout << question;
        }
        *this >> dest;
    }
};

extern thread_local ThreadSafeIOStream threadSafeCout;

#endif
