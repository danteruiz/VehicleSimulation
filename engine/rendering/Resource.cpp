#include "Resource.h"

#include "Backend.h"

#include <spdlog/spdlog.h>
namespace gpu
{
    Resource::Resource(Type type, std::weak_ptr<Backend> backend) :
        m_type(type), m_backend(backend) {}
    Resource::~Resource()
    {
        if (auto backend = m_backend.lock())
        {
            spdlog::debug("releasing object");
            backend->releaseResource(m_id, m_type);
        }
    }
}
