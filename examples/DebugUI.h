#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

struct Entity;
struct Light;
class Window;
class DebugUI
{
public:
    DebugUI(std::shared_ptr<Window> const &window);
    ~DebugUI();
    void show(Entity &entity, Light &light, float deltaTime,
              std::function<void(std::string, bool)> loadNewModel,
              std::function<void(std::string)> generateIBLEnvironments);
    bool focus();
    std::string getModelPath() const { return m_path; }
    bool getRotateCamera() const { return m_rotateCamera; }
    std::string getEnvironmentMapPath() const { return m_environmentMapPath; }
    bool useIrradianceMap() const { return m_useIrradianceMap; }

private:
    float m_lightColor[3];
    bool m_focus { false };
    bool m_useModel { false };
    bool m_rotateCamera { false };
    bool m_useIrradianceMap { false };
    int m_modelCurrentIndex { 0 };
    int m_materialIndex { 0 };
    int m_hdrIndex { 0 };
    std::string m_path;
    std::string m_environmentMapPath;
};
