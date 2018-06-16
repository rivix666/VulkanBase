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
        g_Engine->s_Wp = true;
        break;
    }
    case GLFW_KEY_S:
    {
        g_Engine->s_Sp = true;
        break;
    }
    case GLFW_KEY_A:
    {
        g_Engine->s_Ap = true;
        break;
    }
    case GLFW_KEY_D:
    {
        g_Engine->s_Dp = true;
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
        g_Engine->s_Wp = false;
        break;
    }
    case GLFW_KEY_S:
    {
        g_Engine->s_Sp = false;
        break;
    }
    case GLFW_KEY_A:
    {
        g_Engine->s_Ap = false;
        break;
    }
    case GLFW_KEY_D:
    {
        g_Engine->s_Dp = false;
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
    //#FREE_CAMERA
    //g_Engine->GetCamera()->AdjustHeadingPitch(0.0025f * xpos, 0.0025f * ypos);
    LogD("XMouse "); LogD(xpos); LogD("\n");
    LogD("YMouse "); LogD(ypos); LogD("\n");


    static double oldx = xpos;
    static double oldy = ypos;

   // g_Engine->GetCamera()->MoveCamSpherical(0.0025f * (oldy - ypos), 0.0025f * (oldx - xpos));
}

void input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    LogD("Scroll "); LogD(yoffset); LogD("\n");

   // g_Engine->GetCamera()->ChangeViewSphereRadius(yoffset);
}
