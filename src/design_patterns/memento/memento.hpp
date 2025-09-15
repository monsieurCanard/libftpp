#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include <stddef.h>
#include <string.h>

#include <stdexcept>
#include <vector>

/**
 * @brief Memento Design Pattern
 * @warning You must implement _saveToSnapshot and _loadFromSnapshot
 *
 */
class Memento
{

public:
    class Snapshot
    {
    private:
        std::vector<unsigned char> _buffer;

        // Mutable car modifié même dans les méthodes const
        mutable size_t _cursor = 0;

    public:
        template <typename T>
        Snapshot& operator<<(const T& value)
        {
            const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
            _buffer.insert(_buffer.end(), ptr, ptr + sizeof(T));
            return *this;
        }

        // LECTURE
        template <typename T>
        Snapshot operator>>(T& value) const
        {
            if (sizeof(T) + _cursor > _buffer.size())
                throw std::out_of_range("Buffer overflow on read");

            memcpy(&value, _buffer.data() + _cursor, sizeof(T));
            _cursor += sizeof(T);
            return *this;
        }

        Snapshot& operator<<(const std::string& value);
        Snapshot& operator>>(std::string& value);

        void reset() const;
    };

    Snapshot save();
    void     load(const Memento::Snapshot& state);

private:
    virtual void _saveToSnapshot(Memento::Snapshot& snapshot) const   = 0;
    virtual void _loadFromSnapshot(const Memento::Snapshot& snapshot) = 0;
};

#endif