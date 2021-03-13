#include "Buffer.h"

#include <cstring>
#include <cstdlib>

#include <spdlog/spdlog.h>


Buffer::~Buffer()
{
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}


Buffer& Buffer::operator=(Buffer &buffer) {
    setData(buffer.m_data, buffer.m_size);
    return *this;
}

bool Buffer::setData(void const *data, size_t size)
{
    if (m_data) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_data = new uint8_t[size];
    std::memcpy(m_data, data, size);
    m_size = size;
    m_dirty = true;
    return static_cast<bool>(m_data);
}

bool Buffer::appendData(void const *data, size_t size) {
    if (!m_data) {
        return setData(data, size);
    }

    uint32_t offset = m_size;
    resize(m_size + size);
    std::memcpy(m_data + offset, data, size);
    m_dirty = true;
    return static_cast<bool>(m_data);
}

void Buffer::resize(size_t size)
{
    uint8_t *newData = new uint8_t[size];

    std::memcpy(newData, m_data, m_size);
    m_size = size;
    delete[] m_data;
    m_data = newData;
}

size_t BufferView::getStride() const
{
    return m_format.getDimensionSize();
}
