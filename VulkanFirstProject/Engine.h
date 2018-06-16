#pragma once
#include "VulkanRenderer.h"
#include "GObjectControl.h"

class CEngine
{
public:
    CEngine(GLFWwindow* window);
    ~CEngine();

    bool Init();

    void Frame();
    void UpdateScene();

    inline HWND                GetHwnd() const { return m_Hwnd; }
    inline GLFWwindow*         GetGlfwWindow() const { return m_MainWnd; }
    inline CVulkanRenderer*    GetRenderer() const { return m_Renderer; }
    inline CGObjectControl*    GetObjectControl() const { return m_ObjectControl; }

private:
    HWND        m_Hwnd = nullptr;
    GLFWwindow* m_MainWnd = nullptr;

    CVulkanRenderer* m_Renderer = nullptr;
    CGObjectControl* m_ObjectControl = nullptr;
};

