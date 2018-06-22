#pragma once
#include "Camera.h"
#include "VulkanRenderer.h"
#include "GObjectControl.h"
#include "Utils/Timer.h"

class CEngine
{
public:
    CEngine(GLFWwindow* window);
    ~CEngine();

    // Init
    bool Init();

    // Frame
    void Frame();
    void UpdateScene();

    // Getters
    inline HWND Hwnd() const { return m_Hwnd; }
    inline GLFWwindow* GlfwWindow() const { return m_MainWnd; }
    inline CVulkanRenderer* Renderer() const { return m_Renderer; }
    inline CGObjectControl* ObjectControl() const { return m_ObjectControl; }
    inline VkDevice Device() const { return m_Renderer != nullptr ? m_Renderer->GetDevice() : nullptr; }

    inline CCamera* Camera() const { return m_Camera; }
    inline const CTimer& Timer() const { return m_FrameTimer; }
    inline double LastFrameTime() const { return m_LastFrameTime; }

private:
    // Wnd
    HWND m_Hwnd = nullptr;
    GLFWwindow* m_MainWnd = nullptr;

    // Systems Managers
    CVulkanRenderer* m_Renderer = nullptr;
    CGObjectControl* m_ObjectControl = nullptr;

    // Camera
    CCamera* m_Camera = nullptr;

    // Timer
    double m_LastFrameTime = 0.0;
    CTimer m_FrameTimer = CTimer(ETimerType::MiliSeconds);
};

