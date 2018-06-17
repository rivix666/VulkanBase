#include "stdafx.h"
#include "Engine.h"
#include "Camera.h"
#include "InputsListener.h"

CEngine::CEngine(GLFWwindow* window)
    : m_MainWnd(window)
{
}

CEngine::~CEngine()
{
    if (m_Renderer)
    {
        if (m_ObjectControl)
        {
            m_ObjectControl->Shutdown();
            DELETE(m_ObjectControl);
        }
        m_Renderer->Shutdown();
        DELETE(m_Renderer);

        SAFE_DELETE(m_Camera);
    }
}

bool CEngine::Init()
{
    if (m_MainWnd)
    {
        m_Hwnd = glfwGetWin32Window(m_MainWnd);
        input::InitInputListeners(m_MainWnd);

        m_Renderer = new CVulkanRenderer(m_MainWnd);
        if (!m_Renderer->Init())
            return false;

        m_ObjectControl = new CGObjectControl(m_Renderer->GetDevice());
        m_Camera = new CCamera();
        return true;
    }
    return false;
}

void CEngine::Frame()
{
    // Start frame timer
    m_FrameTimer.startTimer();

    // Update scene
    UpdateScene();

    // Render
    m_Renderer->PresentQueueWaitIdle();
    m_Renderer->Render();

    // Store elapsed time
    m_LastFrameTime = m_FrameTimer.getElapsedTime();
}

void CEngine::UpdateScene()
{
    UpdateCamera();
}

void CEngine::UpdateCamera() //#CAMERA posprzatac
{
    m_Camera->Update();

    UniformBufferObject ubo = {};
    ubo.view = m_Camera->ViewMatrix();
    ubo.proj = m_Camera->ProjectionMatrix();
    ubo.obj_world = glm::mat4(1.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory);
}