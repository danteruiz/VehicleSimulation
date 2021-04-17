#pragma once

#include <memory>
class Backend;
namespace gpu
{
    class Resource
    {
    public:
        enum class Type
        {
            Invalid = 0,
            Buffer,
            Texture,
            FrameBuffer
        };
        Resource() = default;
        Resource(Type type, std::weak_ptr<Backend> backend);
        ~Resource();

        uint32_t m_id;
        Type m_type { Type::Invalid };
    private:
        std::weak_ptr<Backend> m_backend;
    };
}
