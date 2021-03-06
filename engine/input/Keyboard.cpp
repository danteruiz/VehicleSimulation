#include "Keyboard.h"


#include <GlfwApplication.h>
#include <Window.h>


#include <GLFW/glfw3.h>

Keyboard::Keyboard(InputDevice::Type type) noexcept : InputDevice(type) {}


int Keyboard::getButton(int channel)
{
    GLFWwindow* window = GlfwApplication::instance()->getWindow()->getWindowPtr();

    return glfwGetKey(window, channel);
}
