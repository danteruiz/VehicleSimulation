#include "DebugUI.h"

#include "Entity.h"
#include "Helper.h"

#include <imgui/Imgui.h>
#include <imgui.h>
#include <Window.h>
#include <Material.h>
#include <Model.h>
#include <ModelPaths.h>
#include <IBLEnvironments.h>

DebugUI::DebugUI(std::shared_ptr<Window> const &window)
{
    imgui::initialize(window->getWindowPtr());
    m_path = gltfModels::getModelPath(m_modelCurrentIndex);
    m_environmentMapPath = ibl::getIBLEnvironmentPath(m_hdrIndex);
}


DebugUI::~DebugUI()
{
    imgui::uninitialize();
}

void DebugUI::show(Entity &entity, Light &light, float deltaTime,
                   std::function<void(std::string, bool)> loadNewModel,
                   std::function<void(std::string)> generateIBLEnvironments)
{
    m_lightColor[0] = light.color.x;
    m_lightColor[1] = light.color.y;
    m_lightColor[2] = light.color.z;

    m_focus = ImGui::GetIO().WantCaptureMouse;
    imgui::newFrame();
    ImGui::Begin("DebugUI");

    ImGui::Text("deltaTime: %.6f", deltaTime);

    ImGui::Separator();
    ImGui::Text("Camera");
    if (ImGui::RadioButton("Rotating Camera", m_rotateCamera)) {
        m_rotateCamera = true;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("FPS Camera", !m_rotateCamera))
    {
        m_rotateCamera = false;
    }

    ImGui::Separator();
    ImGui::Text("Environment Map");

    std::vector<std::string> iblNames = ibl::getIBLNames();
    if (imgui::Combo("HDR textures", &m_hdrIndex, iblNames))
    {
        m_environmentMapPath = ibl::getIBLEnvironmentPath(m_hdrIndex);
        generateIBLEnvironments(m_environmentMapPath);
    }

    ImGui::Text("Skybox");
    if (ImGui::RadioButton("Irradiance Map", m_useIrradianceMap)) {
        m_useIrradianceMap = true;
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("Regular Map", !m_useIrradianceMap))
    {
        m_useIrradianceMap = false;
    }

    ImGui::Separator();

    ImGui::Text("Light");
    ImGui::Separator();
    //ImGui::SliderFloat("ambient", &light.ambient, 0.0f, 1.0f);
    //ImGui::SliderFloat("intensity", &light.intensity, 0.0f, 6.0f);
    ImGui::SliderFloat("position.x", &light.position.x, -50.0f, 50.0f);
    ImGui::SliderFloat("position.y", &light.position.y, -50.0f, 50.0f);
    ImGui::SliderFloat("position.z", &light.position.z, -50.0f, 50.0f);
    ImGui::ColorEdit3("color", m_lightColor);


    light.color.x = m_lightColor[0];
    light.color.y = m_lightColor[1];
    light.color.z = m_lightColor[2];


    ImGui::Separator();

    ImGui::Text("Entity");
    if (ImGui::RadioButton("Sphere Entity", !m_useModel)) {
        m_useModel = false;
        loadNewModel(gltfModels::getModelPath(m_modelCurrentIndex), m_useModel);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Model Entity", m_useModel))
    {
        m_useModel = true;
        loadNewModel(gltfModels::getModelPath(m_modelCurrentIndex), m_useModel);
    }


    if (m_useModel)
    {
        std::vector<std::string> modelNames = gltfModels::getModelNames();
        if (imgui::Combo("glTF Models", &m_modelCurrentIndex, modelNames))
        {
            loadNewModel(gltfModels::getModelPath(m_modelCurrentIndex), m_useModel);
        }
    }

    ImGui::Separator();
    ImGui::Text("Edit Entity Scale");

    float scale = entity.scale.x;
    ImGui::SliderFloat("Entity Scale", &scale, 0.0f, 30.0f);

    entity.scale.x = scale;
    entity.scale.y = scale;
    entity.scale.z = scale;
    ImGui::Separator();
    ImGui::Text("Edit Entity Materials");


    std::vector<std::string> modelNames = gltfModels::getModelNames();
    if (entity.model) {
        std::vector<std::string> materialNames;

        for (auto& entry : entity.model->m_materials)
        {
            materialNames.push_back(std::to_string(entry.first));
        }

        imgui::Combo("material list", &m_materialIndex, materialNames);

        auto &material = std::get<0>(entity.model->m_materials[m_materialIndex]);
        auto color = material->albedo;
        float entityColor[3] = { color.x, color.y, color.z};
        ImGui::ColorEdit3("entity color", entityColor);
        material->albedo.x = entityColor[0];
        material->albedo.y = entityColor[1];
        material->albedo.z = entityColor[2];

        ImGui::SliderFloat("roughness", &material->roughness, 0.04f, 1.0f);
        ImGui::SliderFloat("metallic", &material->metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("ao", &material->ao, 0.0f, 1.0f);
    }
    ImGui::End();
}

bool DebugUI::focus()
{
    return m_focus;
}
