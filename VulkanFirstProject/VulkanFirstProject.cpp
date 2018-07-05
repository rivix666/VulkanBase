#include "stdafx.h"

// #TMP
#include "GBaseObject.h"

#pragma optimize("")

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
    // Create objects
    REGISTER_OBJ(0, new CGBaseObject(EBaseObjInitType::PLANE));
    REGISTER_OBJ(0, new CGBaseObject(EBaseObjInitType::BOX, glm::vec3(0.0f, 4.0f, 0.0f)));
    g_Engine->Renderer()->RecreateCommandBuffer(); //#CMD_BUFF czy trzeba to wolac co register_obj, moz ejakis update? a co w takim razie z particlami?

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        g_Engine->Frame();

        //#UNI_BUFF
        //////////////////////////////////////////////////////////////////////////

        uint32_t offsets2[2];
        offsets2[0] = 0;
        offsets2[1] = sizeof(SObjUniBuffer);
        g_Engine->ObjectControl()->TestUpdateUniBuff(offsets2);

        //////////////////////////////////////////////////////////////////////////


        utils::UpdateWindowBar(hwnd);
    }

    return Shutdown();
}


