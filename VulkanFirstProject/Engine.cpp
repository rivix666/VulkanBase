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
    }
}

bool CEngine::Init()
{
    if (m_MainWnd)
    {
        m_Hwnd = glfwGetWin32Window(m_MainWnd);
        InitInputsListener();

        m_Renderer = new CVulkanRenderer(m_MainWnd);
        if (!m_Renderer->Init())
            return false;

        m_ObjectControl = new CGObjectControl(m_Renderer->GetDevice());

        m_Camera = new CCamera();

        //Set up projection matrix
        m_Camera->SetPerspectiveProjection(60.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 600.0f);

        return true;
    }
    return false;
}

void CEngine::Frame()
{
    UpdateScene();
    m_Renderer->PresentQueueWaitIdle();
    m_Renderer->Render();
}

void CEngine::UpdateScene()
{
    UpdateCamera();
}

void CEngine::InitInputsListener()
{
    // Set keyboard
    glfwSetKeyCallback(m_MainWnd, input::KeyCallback);

    // Set mouse
    glfwSetCursorPosCallback(m_MainWnd, input::CursorPositionCallback);
    glfwSetMouseButtonCallback(m_MainWnd, input::MouseButtonCallback);
    glfwSetScrollCallback(m_MainWnd, input::ScrollCallback);
    glfwSetInputMode(m_MainWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED/*GLFW_CURSOR_HIDDEN*/);
}

void CEngine::UpdateCamera()
{

    m_Camera->Update();


    UniformBufferObject ubo = {};
    ubo.view = m_Camera->ViewMatrix();
    ubo.proj = m_Camera->ProjectionMatrix();
    ubo.model = glm::mat4(1.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory);
}