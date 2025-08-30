#include "memento.hpp"

Memento::Snapshot Memento::save()
{
    Snapshot snap;
    this->_saveToSnapshot(snap);
    return snap;
}

void Memento::load(const Memento::Snapshot& state)
{
    this->_loadFromSnapshot(state);
}
