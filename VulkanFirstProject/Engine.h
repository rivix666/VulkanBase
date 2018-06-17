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

    bool Init();

    void Frame();
    void UpdateScene();

    inline HWND GetHwnd() const { return m_Hwnd; }
    inline GLFWwindow* GetGlfwWindow() const { return m_MainWnd; }
    inline CVulkanRenderer* GetRenderer() const { return m_Renderer; }
    inline CGObjectControl* GetObjectControl() const { return m_ObjectControl; }
    inline VkDevice GetDevice() const { return m_Renderer != nullptr ? m_Renderer->GetDevice() : nullptr; }

    inline CCamera* GetCamera() const { return m_Camera; }
    inline const CTimer& GetTimer() const { return m_FrameTimer; }
    inline double GetLastFrameTime() const { return m_LastFrameTime; }

protected:
    void InitInputsListener();


    void UpdateCamera();

private:
    // Wnd
    HWND        m_Hwnd = nullptr;
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

