#include "memento.hpp"

Memento::Snapshot Memento::save()
{
    Snapshot snap;
    this->_saveToSnapshot(snap);
    return snap;
}

void Memento::Snapshot::reset()
{
    _cursor = 0;
}

void Memento::load(const Memento::Snapshot& state)
{
    Snapshot tmp = state;
    tmp.reset();
    this->_loadFromSnapshot(tmp);
}

Memento::Snapshot& Memento::Snapshot::operator<<(const std::string& value)
{
    size_t size = value.length();
    *this << size;
    _buffer.insert(_buffer.end(), value.begin(), value.end());
    return *this;
}

Memento::Snapshot& Memento::Snapshot::operator>>(std::string& value)
{
    size_t size;
    *this >> size;
    if (size + _cursor > _buffer.size())
        throw std::runtime_error("Read out of Buffer !");

    value.assign(_buffer.begin() + _cursor, _buffer.begin() + _cursor + size);

    _cursor += size;
    return *this;
}