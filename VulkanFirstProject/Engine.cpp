#include "stdafx.h"
#include "Engine.h"

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

        m_Renderer = new CVulkanRenderer(m_MainWnd);
        if (!m_Renderer->Init())
            return false;

        m_ObjectControl = new CGObjectControl(m_Renderer->GetDevice());

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

}
