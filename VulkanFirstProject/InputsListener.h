#pragma once

namespace input
{
    // Keyboard inputs handle
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void HandlePress(GLFWwindow* window, const int& key, const int& scancode, const int& action, const int& mods);
    void HandleRelease(GLFWwindow* window, const int& key, const int& scancode, const int& action, const int& mods);

    // Mouse inputs handle
    void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


    //#INPUT
    // przerobic to na static clase z vectorem bool wielkosci GLFW_KEY_LAST + 1 i po prostu ustawiac na press tru a na release false;
}