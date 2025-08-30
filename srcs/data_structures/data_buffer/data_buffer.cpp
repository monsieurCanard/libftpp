#include "data_buffer.hpp"

DataBuffer::DataBuffer() : _cursor(0) {}

DataBuffer::~DataBuffer() {}

void DataBuffer::reset()
{
    _cursor = 0;
}
