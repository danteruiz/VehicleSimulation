#pragma once
#include "Buffer.h"

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <inttypes.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


#include <Layout.h>


struct Material;
class Shader;
struct Vertex
{
    Vertex(glm::vec3 p) : position(p) {}
    Vertex(glm::vec3 p, glm::vec3 n) : position(p), normal(n) {}
    Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 uv) : position(p), normal(n), texCoord(uv) {}
    glm::vec3 position;
    glm::vec3 normal { 0.0f, 1.0f, 0.0f };
    glm::vec2 texCoord { 0.0f, 0.0f };
};

enum class Topology : uint8_t
{
    Points = 0,
    Lines,
    LineStrip,
    Triangles
};

struct SubMesh
{
    uint32_t m_startIndex;
    uint32_t m_numIndices;
    uint32_t m_materialIndex;
};

struct Mesh
{
    std::vector<SubMesh> m_subMeshes;
    Buffer::Pointer m_vertexBuffer;
    Buffer::Pointer m_indexBuffer;

    std::unordered_map<Slot, BufferView> m_bufferViews;
    std::vector<Attribute> m_attributes;
    glm::mat4 m_matrix; //maybe make this into a tranform
};

glm::mat4 getLocalMeshMatrix(Mesh const &mesh);


struct Model
{
    using Pointer = std::shared_ptr<Model>;
    std::vector<Mesh> m_meshes;
    std::unordered_map<uint32_t, std::tuple<std::shared_ptr<Material>,
                                            std::shared_ptr<Shader>>> m_materials;
};


enum ModelShape : uint8_t
{
    Cube = 0,
    Sphere,
    Quad,
    NUM_SHAPES
};

class ModelCache
{
public:
    ModelCache();
    Model::Pointer loadModel(std::string const &file);
    Model::Pointer getModelShape(int modelShape);

private:
    std::array<Model::Pointer, ModelShape::NUM_SHAPES> m_modelShapes;
    std::unordered_map<std::string, Model::Pointer> m_models;
};
//Model::Pointer loadModel(std::string const &file);
