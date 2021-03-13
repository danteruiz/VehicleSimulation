#pragma once

#include <memory>
#include <vector>

#include "Resource.h"
#include "Buffer.h"
#include "Layout.h"
class Pipeline;
class State;
class Backend : public std::enable_shared_from_this<Backend>
{
public:
    Backend() = default;
    ~Backend() = default;


    void setVertexBuffer(Buffer::Pointer &buffer);
    void setIndexBuffer(Buffer::Pointer &buffer);
    void enableAttributes(std::vector<Attribute> const &atrributes);

    // void drawIndexArray(Command commad);
    // void drawArrays(Command command);
    // void setPipeline(Pipeline *pipeline);
    // void setState(State* state);
    // void enableAttributes(std::vertor<Attributes> const &atrributes);

    void releaseResource(uint32_t resourceId, gpu::Resource::Type type);

private:
    void syncBuffer(Buffer::Pointer &buffer, uint32_t type);
    std::vector<uint32_t> m_releasedBuffers;
    Pipeline *m_pipline { nullptr };
    State *m_state { nullptr };
};
