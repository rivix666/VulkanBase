#include "stdafx.h"
#include "InputsListener.h"

void input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        HandlePress(window, key, scancode, action, mods);
    else if (action == GLFW_RELEASE)
        HandleRelease(window, key, scancode, action, mods);
}

void input::HandlePress(GLFWwindow* window, const int& key, const int& scancode, const int& action, const int& mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::FORWARD, true);
        break;
    }
    case GLFW_KEY_S:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::BACKWARD, true);
        break;
    }
    case GLFW_KEY_A:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::LEFT, true);
        break;
    }
    case GLFW_KEY_D:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::RIGHT, true);
        break;
    }
    case GLFW_KEY_Q:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::DOWN, true);
        break;
    }
    case GLFW_KEY_E:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::UP, true);
        break;
    }
    case GLFW_KEY_LEFT_SHIFT:
    {
        g_Engine->GetCamera()->SetMoveSpeed(0.5f);
        break;
    }
    case GLFW_KEY_LEFT_CONTROL:
    {
        g_Engine->GetCamera()->SetMoveSpeed(0.02f);
        break;
    }
    }
}

void input::HandleRelease(GLFWwindow* window, const int& key, const int& scancode, const int& action, const int& mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::FORWARD, false);
        break;
    }
    case GLFW_KEY_S:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::BACKWARD, false);
        break;
    }
    case GLFW_KEY_A:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::LEFT, false);
        break;
    }
    case GLFW_KEY_D:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::RIGHT, false);
        break;
    }
    case GLFW_KEY_Q:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::DOWN, false);
        break;
    }
    case GLFW_KEY_E:
    {
        g_Engine->GetCamera()->MoveFreeCam(ECamMoveDir::UP, false);
        break;
    }
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_LEFT_CONTROL:
    {
        g_Engine->GetCamera()->SetMoveSpeed(0.1f);
        break;
    }
    case GLFW_KEY_SPACE:
    {
        g_Engine->GetCamera()->SetUseFreeCam(!g_Engine->GetCamera()->UseFreeCam());
        break;
    }
    case GLFW_KEY_ESCAPE:
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    }
    }
}

void input::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    static double oldx = xpos;
    static double oldy = ypos;

    g_Engine->GetCamera()->ProcessMouseMoveInput(xpos - oldx, oldy - ypos);
    oldx = xpos;
    oldy = ypos;
}

void input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_Engine->GetCamera()->ChangeViewSphereRadius(yoffset);
}
