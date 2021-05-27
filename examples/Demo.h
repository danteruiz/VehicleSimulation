#pragma once

#include "GlfwApplication.h"
#include <memory>
#include <vector>

#include "Entity.h"
#include "DebugDraw.h"


#include <Skybox.h>



class Window;
class Buffer;
class DebugUI;
class Shader;
class ModelCache;
class Backend;
class DebugDraw;
struct Marker;

class DemoApplication : public GlfwApplication
{
public:
    DemoApplication();
    void exec() override;
private:

    void generateIBLEnvironment(std::string& texturePath);
    unsigned int generateEnviromentMap();
    Light m_light;
    Entity m_modelEntity;
    Skybox m_skybox;
    std::vector<Light> m_lights;
    std::shared_ptr<ModelCache> m_modelCache;
    std::shared_ptr<DebugUI> m_debugUI;
    std::shared_ptr<Shader> m_pipeline { nullptr };
    std::shared_ptr<Shader> m_irradiance { nullptr };
    std::shared_ptr<Shader> m_convertToCubeMap { nullptr };
    std::shared_ptr<Shader> m_filterMap { nullptr };
    std::shared_ptr<Shader> m_brdfLut { nullptr };
    std::shared_ptr<DebugDraw> m_debugDraw { nullptr };
    std::vector<Marker> m_markers;
    std::shared_ptr<Backend> m_backend { nullptr };
};
