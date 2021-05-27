#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Buffer;
class Shader;
class Backend;
struct Attribute;

struct Marker
{
    glm::mat4 m_matrix;
};

class DebugDraw
{
public:
    DebugDraw();
    ~DebugDraw() = default;
    void renderMarkers(std::vector<Marker> const &markers, glm::mat4 const &view,
                       glm::mat4 const &projection, Backend *backend);
private:
    std::shared_ptr<Buffer> m_vertexBuffer { nullptr };
    std::vector<Attribute> m_attributes;
    std::vector<Marker> m_makers;
    std::shared_ptr<Shader> m_debugPipeline { nullptr };
};
