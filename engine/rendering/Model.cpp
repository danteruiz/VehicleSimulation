#include "Model.h"

#include "Buffer.h"
#include "Shader.h"
#include "Material.h"
#include "Format.h"
#include "Layout.h"
#include "Texture.h"

#define TINYGLTF_IMPLEMENTATION
#include "external/tiny_gltf.h"

#include <spdlog/spdlog.h>
#include <StopWatch.h>

#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <vector>
#include <string>


std::string def = "#define HAS_";
static std::string resources = RESOURCE_PATH;
static const std::string shaderPath = std::string(RESOURCE_PATH) + "shaders/";
static const std::string VERTEX_SHADER = shaderPath + "pbr.vs";
static const std::string FRAGMENT_SHADER = shaderPath + "pbr.fs";

enum class GLTFAttributeType : uint8_t
{
    Position = 0,
    Normal,
    TexCoord,
    Invalid
};

std::shared_ptr<Texture> loadMaterialTexture(tinygltf::Model &model, int index,
                                             std::string materialName,
                                             std::string& defines)
{
    if (index < 0)
    {
        return nullptr;
    }

    tinygltf::Texture const &gltfTexture = model.textures[index];
    tinygltf::Image &image = model.images[gltfTexture.source];

    defines += def + materialName + ";\n";
    return createTextureFromGLTF(image.width, image.height, image.component,
                                 image.bits, &image.image.at(0));
}

template <typename T>
void processIndexData(T const *gltfIndices, std::vector<uint32_t> &indices,
                      size_t count, size_t startIndex)
{

    uint32_t max = 0;
    for (size_t index = 0; index < count; index++)
    {
        uint32_t indice = static_cast<uint32_t>(gltfIndices[index])
            + static_cast<uint32_t>(startIndex);

        max = std::max(indice, max);
        indices.push_back(indice);
    }
}

GLTFAttributeType getGLTFAttributeFromString(std::string const &attribute)
{
    if (attribute == "POSITION") {
        return GLTFAttributeType::Position;
    } else if (attribute == "NORMAL") {
        return GLTFAttributeType::Normal;
    } else if (attribute == "TEXCOORD_0") {
        return GLTFAttributeType::TexCoord;
    }

    return GLTFAttributeType::Invalid;
}

void addAttributesToArray(int attributeIndex, std::vector<float> &buffer, tinygltf::Model &gltfModel)
{
    tinygltf::Accessor const &accessor = gltfModel.accessors[attributeIndex];
    tinygltf::BufferView const &bufferView = gltfModel.bufferViews[accessor.bufferView];
    tinygltf::Buffer &gltfBuffer = gltfModel.buffers[bufferView.buffer];

    float const *bufferData = reinterpret_cast<float*>(&gltfBuffer.data[bufferView.byteOffset +
                                                                        accessor.byteOffset]);

    int numComponents = tinygltf::GetNumComponentsInType(accessor.type);
    size_t size = accessor.count * numComponents;
    for (size_t index = 0; index < size; ++index)
    {
        buffer.push_back(bufferData[index]);
    }
}

Mesh processMesh(tinygltf::Model &model, tinygltf::Mesh& gltfMesh)
{
    Mesh mesh;

    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<uint32_t> indices;

    for (size_t i = 0; i < gltfMesh.primitives.size(); ++i)
    {
        SubMesh subMesh;
        subMesh.m_startIndex  = static_cast<uint32_t>(indices.size());
        tinygltf::Primitive primitive = gltfMesh.primitives[i];

        for (auto attribute : primitive.attributes)
        {
            GLTFAttributeType attributeType = getGLTFAttributeFromString(attribute.first);
            int attributeIndex = attribute.second;

            switch (attributeType)
            {
                case GLTFAttributeType::Position:
                    addAttributesToArray(attributeIndex, positions, model);
                    break;

                case GLTFAttributeType::Normal:
                    addAttributesToArray(attributeIndex, normals, model);
                    break;

                case GLTFAttributeType::TexCoord:
                    addAttributesToArray(attributeIndex, texCoords, model);
                    break;

                default:
                    break;
            }
        }

        if (primitive.indices >= 0)
        {
            const tinygltf::Accessor &indexAccessor =
                model.accessors[primitive.indices];
            const tinygltf::BufferView &indexBufferView =
                model.bufferViews[indexAccessor.bufferView];
            tinygltf::Buffer &indexBuffer =
                model.buffers[indexBufferView.buffer];

            void* const indexData =
                &indexBuffer.data[indexBufferView.byteOffset
                                  + indexAccessor.byteOffset];

            switch (indexAccessor.componentType)
            {
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                    processIndexData<uint8_t>(reinterpret_cast<uint8_t*>(indexData), indices,
                                              indexAccessor.count, subMesh.m_startIndex);
                    break;

                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                    processIndexData<uint16_t>(reinterpret_cast<uint16_t*>(indexData), indices,
                                               indexAccessor.count, subMesh.m_startIndex);
                    break;

                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                case TINYGLTF_COMPONENT_TYPE_FLOAT:
                case TINYGLTF_COMPONENT_TYPE_INT:
                    processIndexData<uint32_t>(reinterpret_cast<uint32_t*>(indexData), indices,
                                               indexAccessor.count, subMesh.m_startIndex);
                    break;

                default:
                    std::cout << "UNSUPPORTED TYPE: " << indexAccessor.componentType << std::endl;
            }
            subMesh.m_numIndices = static_cast<uint32_t>(indexAccessor.count);
        } else {
            spdlog::debug("No indices!!!!!!!");
        }
        subMesh.m_materialIndex = primitive.material;
        mesh.m_subMeshes.push_back(subMesh);
    }

    mesh.m_vertexBuffer = std::make_shared<Buffer>();
    mesh.m_vertexBuffer->setData(reinterpret_cast<uint8_t*>(&positions[0]),
                                 sizeof(float) * positions.size());

    mesh.m_vertexBuffer->appendData(reinterpret_cast<uint8_t*>(&normals[0]),
                                    sizeof(float) * normals.size());

    mesh.m_vertexBuffer->appendData(reinterpret_cast<uint8_t*>(&texCoords[0]),
                                    sizeof(float) * texCoords.size());

    mesh.m_indexBuffer = std::make_shared<Buffer>();

    mesh.m_indexBuffer->setData(indices.data(),
                                   sizeof(uint32_t) * indices.size());

    Attribute positionAttribute(Slot::Position, {Type::Float, Dimension::Vec3}, 0);
    Attribute normalAttribute(Slot::Normal, {Type::Float, Dimension::Vec3}, positions.size());
    Attribute texCoordAttribute(Slot::TexCoord, {Type::Float, Dimension::Vec2},
                                normalAttribute.m_offset + normals.size());

    mesh.m_attributes = {
        positionAttribute,
        normalAttribute,
        texCoordAttribute
    };

    return mesh;
}



glm::mat4 calculateLocalMatrix(glm::vec3& translation, glm::vec3& scale,
                               glm::quat& rotation)
{
    return glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation)
        * glm::scale(glm::mat4(1.0f), scale);//* matrix;
}

glm::mat4 extractMatrix(tinygltf::Node &node, glm::mat4 const &parentMatrix)
{
    glm::mat4 matrix(1.0f);
    if (node.matrix.size() == 16)
    {
        matrix = glm::make_mat4x4(node.matrix.data());
    }

    glm::vec3 translation(0.0f);
    if (node.translation.size() == 3)
    {
        translation = glm::make_vec3(node.translation.data());
    }

    glm::quat rotation;
    if (node.rotation.size() == 4)
    {
        rotation = glm::make_quat(node.rotation.data());
    }

    glm::vec3 scale(1.0f);
    if (node.scale.size() == 3)
    {
        scale = glm::make_vec3(node.scale.data());
    }

    return parentMatrix * calculateLocalMatrix(translation, scale, rotation);
}

void processNode(tinygltf::Model &gltfModel, tinygltf::Node &node, std::shared_ptr<Model> &model,
                 glm::mat4 parentMatrix = glm::mat4(1.0f))
{
    glm::mat4 finalMatrix = extractMatrix(node, parentMatrix);

    if ((node.mesh >= 0) && (node.mesh < (int) gltfModel.meshes.size()))
    {
        Mesh mesh = processMesh(gltfModel, gltfModel.meshes[node.mesh]);
        mesh.m_matrix = finalMatrix;
        model->m_meshes.push_back(mesh);
    }

    for (size_t i = 0; i < node.children.size(); ++i)
    {
        processNode(gltfModel, gltfModel.nodes[node.children[i]], model, finalMatrix);
    }
}

void getShadersAndMaterials(std::shared_ptr<Model>& model, tinygltf::Model gltfModel)
{
    size_t materialCount = gltfModel.materials.size();
    for (size_t index = 0; index < materialCount; ++index)
    {
        const auto& gltfMaterial = gltfModel.materials[index];
        for (auto ext: gltfMaterial.extensions) {
            spdlog::debug("model: extensino {}", ext.first);
        }

        std::string defines;
        std::shared_ptr<Material> material = std::make_shared<Material>();
        auto pbrMaterial = gltfMaterial.pbrMetallicRoughness;
        auto pbrBaseColor = pbrMaterial.baseColorFactor;
        material->albedo = glm::vec3(pbrBaseColor[0], pbrBaseColor[1], pbrBaseColor[2]);
        material->ao = (float) pbrBaseColor[3];
        auto emissiveFactor = gltfMaterial.emissiveFactor;
        material->emissive = glm::vec3(emissiveFactor[0], emissiveFactor[1], emissiveFactor[2]);
        material->roughness = (float) pbrMaterial.roughnessFactor;
        material->metallic = (float) pbrMaterial.metallicFactor;
        material->albedoTexture = loadMaterialTexture(gltfModel, pbrMaterial.baseColorTexture.index,
                                                      "ALBEDO_MAP", defines);

        material->normalTexture = loadMaterialTexture(gltfModel, gltfMaterial.normalTexture.index,
                                                      "NORMAL_MAP", defines);

        material->emissiveTexture = loadMaterialTexture(gltfModel,
                                                        gltfMaterial.emissiveTexture.index,
                                                        "EMISSIVE_MAP", defines);

        material->occlusionTexture = loadMaterialTexture(gltfModel,
                                                         gltfMaterial.occlusionTexture.index,
                                                         "OCCLUSION_MAP", defines);

        material->metallicTexture = loadMaterialTexture(gltfModel,
                                                        pbrMaterial.metallicRoughnessTexture.index,
                                                        "METALLIC_ROUGHNESS_MAP", defines);

        std::shared_ptr<Shader> shader = std::make_shared<Shader>(FRAGMENT_SHADER, VERTEX_SHADER,
                                                                  defines);
        auto tuple = std::make_tuple(material, shader);
        model->m_materials[(uint32_t) index] = tuple;
    }
}

tinygltf::Model loadGLTFModelFile(std::string const &file, bool &success)
{
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    success = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, file);

    if (!warn.empty())
    {
        std::cout << "Warn: " << warn << std::endl;
    }

    if (!err.empty())
    {
        std::cout << "Err: " << err << std::endl;
    }

    return gltfModel;
}


Model::Pointer loadModelFromFile(std::string const &file)
{
    bool success;

    tinygltf::Model gltfModel= loadGLTFModelFile(file, success);

    if (!success)
    {
        return nullptr;
    }

    Model::Pointer model = std::make_shared<Model>();
    const tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i)
    {
        processNode(gltfModel, gltfModel.nodes[scene.nodes[i]], model);
    }

    getShadersAndMaterials(model, gltfModel);

    return model;
}

static float const  PI = 3.14159265359;

static int const X_SEGMENTS = 64.0f;
static int const Y_SEGMENTS = 64.0f;
Model::Pointer buildSphere()
{
    Model::Pointer model = std::make_shared<Model>();

    std::vector<float> positions;
    std::vector<float> normals;

    for (int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;

            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(xPos);
            positions.push_back(yPos);
            positions.push_back(zPos);

            normals.push_back(xPos);
            normals.push_back(yPos);
            normals.push_back(zPos);
        }
    }

    std::vector<int> indices;
    for (int i = 0; i < Y_SEGMENTS; ++i)
    {
        int k1 = i * (X_SEGMENTS + 1);
        int k2 = k1 + X_SEGMENTS + 1;

        for (int j = 0; j < X_SEGMENTS; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 +1);
            }

            if (i != (Y_SEGMENTS -1))
            {
                indices.push_back(k1 +1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }


    Mesh mesh;
    mesh.m_vertexBuffer = std::make_shared<Buffer>();
    mesh.m_indexBuffer = std::make_shared<Buffer>();

    Buffer::Pointer &vertexBuffer = mesh.m_vertexBuffer;
    vertexBuffer->setData(reinterpret_cast<uint8_t const *>(&positions[0]),
                          sizeof(float) * positions.size());

    vertexBuffer->appendData(reinterpret_cast<uint8_t const *>(&normals[0]),
                           sizeof(float) * normals.size());

    mesh.m_indexBuffer->setData(reinterpret_cast<uint8_t const*>(&indices[0]),
                                sizeof(int) * indices.size());
    SubMesh subMesh;

    subMesh.m_startIndex = 0;
    subMesh.m_numIndices = indices.size();
    subMesh.m_materialIndex = 0;
    mesh.m_subMeshes.push_back(subMesh);

    Attribute positionAttribute(Slot::Position, {Type::Float, Dimension::Vec3}, 0);
    Attribute normalAttribute(Slot::Normal, {Type::Float, Dimension::Vec3}, positions.size());
    mesh.m_attributes = {
        positionAttribute,
        normalAttribute
    };

    model->m_meshes.push_back(mesh);

    return model;
}

Model::Pointer buildCube()
{
    Model::Pointer model = std::make_shared<Model>();


    // Mesh mesh;
    std::vector<float> positions = {
        // right side 0 - 3
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        //top side 4 - 7
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,

        //bottom side 8 - 11
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,

        // left 12 - 15
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,

        // front 16 - 19
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        //back 20 - 23
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
    };



    std::vector<float> normals = {
        // right side 0 - 3
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        //top side 4 - 7
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        //bottom side 8 - 11
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // left 12 - 15
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // front 16 - 19
        0.0f, 0.0, -1.0f,
        0.0f, 0.0, -1.0f,
        0.0f, 0.0, -1.0f,
        0.0f, 0.0, -1.0f,

        //back 20 - 23
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };


    std::vector<uint32_t> indices = {
        // right side
        0, 1, 2,
        2, 1, 3,
        //top side
        4, 5 , 6,
        4, 6, 7,
        // bottom
        8, 9, 10,
        8, 10, 11,
        //left
        12, 13, 14,
        13, 14, 15,
        //front
        16, 17, 18,
        16, 18, 19,
        //back
        20, 21, 22,
        20, 22, 23
    };


    Mesh mesh;
    mesh.m_vertexBuffer = std::make_shared<Buffer>();
    mesh.m_indexBuffer = std::make_shared<Buffer>();

    Buffer::Pointer &vertexBuffer = mesh.m_vertexBuffer;
    vertexBuffer->setData(reinterpret_cast<uint8_t const *>(&positions[0]),
                          sizeof(float) * positions.size());

    vertexBuffer->appendData(reinterpret_cast<uint8_t const *>(&normals[0]),
                             sizeof(float) * normals.size());

    mesh.m_indexBuffer->setData(reinterpret_cast<uint8_t const*>(&indices[0]),
                                sizeof(uint32_t) * indices.size());

    SubMesh subMesh;

    subMesh.m_startIndex = 0;
    subMesh.m_numIndices = indices.size();
    subMesh.m_materialIndex = 0;
    mesh.m_subMeshes.push_back(subMesh);

    Attribute positionAttribute(Slot::Position, {Type::Float, Dimension::Vec3}, 0);
    Attribute normalAttribute(Slot::Normal, {Type::Float, Dimension::Vec3}, positions.size());
    mesh.m_attributes = {
        positionAttribute,
        normalAttribute
    };

    model->m_meshes.push_back(mesh);

    return model;
}




Model::Pointer buildQuad()
{
    Model::Pointer model  = std::make_shared<Model>();

    // Mesh mesh;

    std::array<float, 12> constexpr positions {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    std::array<float, 12> constexpr normals {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    std::array<float, 8> constexpr texCoords {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    std::array<uint32_t, 6> constexpr indices = {
        0, 1, 2,
        1, 2, 3
    };

    Mesh mesh;
    mesh.m_vertexBuffer = std::make_shared<Buffer>();
    mesh.m_indexBuffer = std::make_shared<Buffer>();

    Buffer::Pointer &vertexBuffer = mesh.m_vertexBuffer;
    vertexBuffer->setData(reinterpret_cast<uint8_t const *>(&positions[0]),
                          sizeof(float) * positions.size());

    vertexBuffer->appendData(reinterpret_cast<uint8_t const *>(&normals[0]),
                             sizeof(float) * normals.size());

    vertexBuffer->appendData(reinterpret_cast<uint8_t const *>(&texCoords[0]),
                             sizeof(float) * texCoords.size());

    mesh.m_indexBuffer->setData(reinterpret_cast<uint8_t const*>(&indices[0]),
                                sizeof(uint32_t) * indices.size());

    SubMesh subMesh;
    subMesh.m_startIndex = 0;
    subMesh.m_numIndices = 6;
    subMesh.m_materialIndex = 0;
    mesh.m_subMeshes.push_back(subMesh);

    Attribute positionAttribute(Slot::Position, {Type::Float, Dimension::Vec3}, 0);
    Attribute normalAttribute(Slot::Normal, {Type::Float, Dimension::Vec3}, positions.size());
    Attribute texCoordAttribute(Slot::TexCoord, {Type::Float, Dimension::Vec2},
                                normalAttribute.m_offset + normals.size());
    mesh.m_attributes = {
        positionAttribute,
        normalAttribute,
        texCoordAttribute
    };

    model->m_meshes.push_back(mesh);
    return model;
}

ModelCache::ModelCache()
{
    m_modelShapes[ModelShape::Cube] = buildCube();
    m_modelShapes[ModelShape::Sphere] = buildSphere();
    m_modelShapes[ModelShape::Quad] = buildQuad();
}


Model::Pointer ModelCache::loadModel(std::string const &file)
{
    auto modelIter = m_models.find(file);

    if (modelIter != m_models.end())
    {
        return modelIter->second;
    }

    Model::Pointer model = nullptr;

    model = loadModelFromFile(file);
    m_models[file] = model;
    return model;
}

Model::Pointer ModelCache::getModelShape(int modelCache)
{
    return m_modelShapes[modelCache];
}
