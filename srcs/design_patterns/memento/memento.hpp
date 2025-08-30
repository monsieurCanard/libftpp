#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include <stddef.h>

#include <stdexcept>
#include <vector>

/*
 * @brief Memento Design Pattern
 * @note Need to be implemented as friend class
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
        size_t                     _cursor = 0;

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
        Snapshot& operator>>(T& value)
        {
            if (sizeof(T) + _cursor > _buffer.size())
                throw std::runtime_error("Read out of Buffer !");

            memcpy(&value, _buffer.data() + _cursor, sizeof(T));
            _cursor += sizeof(T);
            return *this;
        }
    };
    Snapshot save();
    void     load(const Memento::Snapshot& state);

private:
    virtual void _saveToSnapshot(Memento::Snapshot& snapshot) const   = 0;
    virtual void _loadFromSnapshot(const Memento::Snapshot& snapshot) = 0;
};

#endif