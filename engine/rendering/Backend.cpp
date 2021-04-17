#include "Backend.h"

#include "GL.h"

#include <spdlog/spdlog.h>

void Backend::setVertexBuffer(Buffer::Pointer &buffer)
{
    syncBuffer(buffer, GL_ARRAY_BUFFER);
}

void Backend::setIndexBuffer(Buffer::Pointer &buffer)
{
    syncBuffer(buffer, GL_ELEMENT_ARRAY_BUFFER);
}

void Backend::syncBuffer(Buffer::Pointer &buffer, uint32_t bufferType)
{
    auto &gpuResource = buffer->m_gpuResource;
    if (!gpuResource)
    {
        gpuResource.reset(new gpu::Resource(gpu::Resource::Type::Buffer,
                                            shared_from_this()));
        glGenBuffers(1, &gpuResource->m_id);
    }

    glBindBuffer(bufferType, gpuResource->m_id);
    if (buffer->m_dirty && buffer->m_data)
    {
        glBufferData(bufferType, buffer->m_size, static_cast<void*>(buffer->m_data),
                     GL_DYNAMIC_DRAW);
        auto error = glGetError();
        buffer->m_dirty = false;
    }
}

void Backend::releaseResource(uint32_t resourceId, gpu::Resource::Type type)
{
    switch (type)
    {
        case gpu::Resource::Type::Buffer:
            m_releasedBuffers.push_back(resourceId);
            break;
        case gpu::Resource::Type::Invalid:
        default:
            spdlog::error("render: trying to release invalid resource");
            break;
    }
}

void Backend::enableAttributes(std::vector<Attribute> const &attributes) {
    for (auto const &attribute: attributes) {
        glEnableVertexAttribArray(static_cast<GLuint>(attribute.m_slot));

        Format const &format = attribute.m_format;
        glVertexAttribPointer(static_cast<GLuint>(attribute.m_slot),
                              format.getDimensionSize(),
                              GL_FLOAT, GL_FALSE, format.getStride(),
                              (void*) attribute.getTotalOffset());
    }
}
