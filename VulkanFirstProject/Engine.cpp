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
     //#INPUT to wylcizanie do camery zmoe przeniesc jak bedzie juz vector
     if (s_Wp)
     {
         m_Camera->AddToEye(0.0f, 0.0f, 0.1f);
     }
     if (s_Sp)
     {
         m_Camera->AddToEye(0.0f, 0.0f, -0.1f);
     }
     if (s_Ap)
     {
         m_Camera->AddToEye(-0.1f, 0.0f, 0.0f);
     }
     if (s_Dp)
     {
         m_Camera->AddToEye(0.1f, 0.0f, 0.0f);
     }
 
     m_Camera->Update();
 
 
     UniformBufferObject ubo = {};
     ubo.view = m_Camera->ViewMatrix();
     ubo.proj = m_Camera->ProjectionMatrix();
   //  ubo.proj[1][1] *= -1;

//      ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//      ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

 
     void* data;
     vkMapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory, 0, sizeof(ubo), 0, &data);
     memcpy(data, &ubo, sizeof(ubo));
     vkUnmapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory);
 }
 
 
 
 
 
 
 
 
 
 




//#define GLM_FORCE_RADIANS
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <chrono>
//
//void CEngine::UpdateCamera()
//{
//  static auto startTime = std::chrono::high_resolution_clock::now();
//
//  auto currentTime = std::chrono::high_resolution_clock::now();
//  float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//
//  UniformBufferObject ubo = {};
//  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//  ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//  ubo.proj = glm::perspective(glm::radians(45.0f), m_Renderer->GetSwapChainExtent().width / (float)m_Renderer->GetSwapChainExtent().height, 0.1f, 10.0f);
//  ubo.proj[1][1] *= -1;
//
//  void* data;
//  vkMapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory, 0, sizeof(ubo), 0, &data);
//  memcpy(data, &ubo, sizeof(ubo));
//  vkUnmapMemory(m_Renderer->GetDevice(), m_Renderer->m_UniformBufferMemory);
//}