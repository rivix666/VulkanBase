#include "stdafx.h"

// #TMP
#include "GBaseObject.h"

GLFWwindow* window = nullptr;
HWND hwnd = nullptr;

// Window hanlde
//////////////////////////////////////////////////////////////////////////
bool InitWindow()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    hwnd = glfwGetWin32Window(window);

    if (!window || !hwnd)
    {
        utils::FatalError(hwnd, "Application could not create window");
        return false;
    }
}

// Engine
//////////////////////////////////////////////////////////////////////////
bool InitEngine()
{
    if (!g_Engine)
        g_Engine = new CEngine(window);

    if (!g_Engine->Init())
    {
        utils::FatalError(hwnd, "Engine could not be created");
        return false;
    }

    return true;
}

// Shutdown
//////////////////////////////////////////////////////////////////////////
int Shutdown()
{
    SAFE_DELETE(g_Engine);

    if (window)
        glfwDestroyWindow(window);

    return 0;
}

// Main
//////////////////////////////////////////////////////////////////////////
int _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    glfwInit();

    if (!InitWindow())
        return Shutdown();

    if (!InitEngine())
        return Shutdown();

    // #TMP
    REGISTER_OBJ(0, new CGBaseObject());
    g_Engine->GetRenderer()->RecreateCommandBuffer(); //#CMD_BUFF czy trzeba to wolac co register_obj, moz ejakis update? a co w takim razie z particlami?

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        g_Engine->Frame();
        utils::UpdateWindowBar(hwnd);
    }

    return Shutdown();
}


