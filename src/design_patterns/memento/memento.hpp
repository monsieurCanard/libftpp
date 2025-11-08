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
 * @code
 * class MyClass : public Memento
 * {
 * private:
 *   int         _value;
 *   std::string _name;
 *   void _saveToSnapshot(Memento::Snapshot& snapshot) const override
 *   {
 *       snapshot << _value << _name;
 *   }
 *   void _loadFromSnapshot(Memento::Snapshot& snapshot) override
 *   {
 *       snapshot >> _value >> _name;
 *   }
 * public:
 *  MyClass(int value, const std::string& name) : _value(value),
 *                                               _name(name) {}
 * };
 * @endcode
 * @note Basic Usage
 * @code
 * MyClass obj(42, "Initial");
 * Memento::Snapshot snapshot = obj.save();
 *
 * // Modify object state
 * obj = MyClass(100, "Modified");
 *
 * // Restore previous state
 * obj.load(snapshot);
 *
 * // Now obj has _value = 42 and _name = "Initial"
 * @endcode
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
        Snapshot operator>>(T& value)
        {
            if (sizeof(T) + _cursor > _buffer.size())
                throw std::out_of_range("Buffer overflow on read");

            memcpy(&value, _buffer.data() + _cursor, sizeof(T));
            _cursor += sizeof(T);
            return *this;
        }

        Snapshot& operator<<(const std::string& value);
        Snapshot& operator>>(std::string& value);

        void reset();
    };

    Snapshot save();
    void     load(const Memento::Snapshot& state);

private:
    virtual void _saveToSnapshot(Memento::Snapshot& snapshot) const = 0;
    virtual void _loadFromSnapshot(Memento::Snapshot& snapshot)     = 0;
};

#endif