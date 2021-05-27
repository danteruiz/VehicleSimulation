#include "DebugDraw.h"

#include "Backend.h"
#include "Buffer.h"
#include "Layout.h"
#include "Shader.h"

#include <cstdint>


DebugDraw::DebugDraw()
{
    std::array<float, 18> constexpr positions = {
        0.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f
    };

    std::array<float, 18> constexpr colors = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    m_vertexBuffer = std::make_shared<Buffer>();

    m_vertexBuffer->setData(reinterpret_cast<uint8_t const *>(&positions[0]),
                           sizeof(float) * positions.size());
    m_vertexBuffer->appendData(reinterpret_cast<uint8_t const*>(&colors[0]),
                              sizeof(float) * colors.size());

    Attribute positionAttribute(Slot::Position, Format(Type::Float, Dimension::Vec3), 0);
    Attribute colorAttribute(Slot::Color, Format(Type::Float, Dimension::Vec3), positions.size());

    m_attributes = {
        positionAttribute,
        colorAttribute
    };
    // m_debugPipeline = std::make_shared<Shader>(debugFragmentShader, debugVertexShader);
}


void DebugDraw::renderMarkers(std::vector<Marker> const& marker, glm::mat4 const &view,
                              glm::mat4 const &projection, Backend *backend) {

    backend->setVertexBuffer(m_vertexBuffer);
    backend->enableAttributes(m_attributes);

    m_debugPipeline->bind();
    m_debugPipeline->setUniformMat4("view", view);
    m_debugPipeline->setUniformMat4("projection", projection);
    for (auto const &marker: m_makers)
    {
        
    }
}
