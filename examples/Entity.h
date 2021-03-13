#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Material.h>


struct Model;
struct Material;

struct Transform
{
    glm::vec3 translation { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale { 1.0f, 1.0f, 1.0f };
    glm::quat rotation { 1.0f, 0.0f, 0.0f, 0.0f };
};


struct Entity
{
    using Pointer = std::shared_ptr<Entity>;
    std::string name { "Entity" };
    glm::vec3 translation { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale { 1.0f, 1.0f, 1.0f };
    glm::quat rotation { 1.0f, 0.0f, 0.0f, 0.0f };

    std::shared_ptr<Material> material { nullptr };
    std::shared_ptr<Model> model { nullptr };
};


struct Light
{
    float intensity { 1.0f };
    float ambient { 1.0f };
    glm::vec3 color { 1.0f, 1.0f, 1.0f};
    glm::vec3 position { 1.0f, 1.0f, 1.0f };
};
