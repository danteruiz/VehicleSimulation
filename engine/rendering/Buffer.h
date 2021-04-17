#pragma once

#include <memory>
#include <cstdint>

#include "Format.h"
#include "Resource.h"

class Buffer
{
public:
    using Pointer = std::shared_ptr<Buffer>;

    Buffer() = default;
    ~Buffer();

    Buffer& operator=(Buffer &buffer);
    bool setData(void const *data, size_t size);
    bool appendData(void const *data, size_t size);

    void resize(size_t size);

    std::unique_ptr<gpu::Resource> m_gpuResource { nullptr };
    size_t m_size { 0 };
    uint8_t *m_data { nullptr };
    bool m_dirty { false };
};


struct BufferView
{
    BufferView(size_t offset, size_t size, Format format) :
        m_offset(offset), m_size(size), m_format(format) {}

    size_t getStride() const;

    size_t m_offset;
    size_t m_size;
    Format m_format;
};
