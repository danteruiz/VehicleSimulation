#pragma once


#include <memory>

struct Texture;

struct Material
{
    using Pointer = std::shared_ptr<Material>;

    glm::vec3 albedo { 1.0f, 1.0f, 1.0f};
    glm::vec3 emissive { 0.0f, 0.0f, 0.0f };
    float roughness { 1.0f };
    float metallic { 1.0f };
    float opacity { 1.0f };
    float ao { 1.0f };

    std::shared_ptr<Texture> albedoTexture;
    std::shared_ptr<Texture> normalTexture;
    std::shared_ptr<Texture> occlusionTexture;
    std::shared_ptr<Texture> emissiveTexture;
    std::shared_ptr<Texture> metallicTexture;
};
