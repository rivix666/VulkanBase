#include "stdafx.h"
#include "Camera.h"

CCamera::CCamera()
{
    for (uint i = 0; i < (uint)ECamMoveDir::_COUNT_; i++)
        m_MoveDirKeyState[i] = false;
}

void CCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float zNear, float zFar)
{
    FOV = FOV * (float)DEG_TO_RAD;
    m_ProjectionMtx = glm::perspective(FOV, aspectRatio, zNear, zFar);
    m_InvProjectionMtx = glm::inverse(m_ProjectionMtx);
}

void CCamera::Update()
{
    UpdateCameraVectors();

    if (m_UseFreeCam)
        m_ViewMtx = glm::lookAt(m_Eye, m_Eye + m_View, m_Up);
    else
        m_ViewMtx = glm::lookAt(m_Eye, m_View, m_Up);
}

void CCamera::SetUseFreeCam(bool use /*= true*/)
{
    static glm::vec3 free_view(1.0);
    static glm::vec3 free_eye(1.0);
    if (use)
    {
        m_Yaw = -90.0f;
        m_Pitch = 0.0f;;
        m_View = free_view;
        m_Eye = free_eye;
    }
    else
    {
        free_eye = CameraPosition();
        free_view = CameraView();
        m_SphereCamPSI = -0.5f;
        m_SphereCamFI = 0.0f;
        m_SphereCamRadius = -10.0f;;
        m_Eye = glm::vec3(0.0f, 5.0f, -25.0f);
        m_View = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    m_UseFreeCam = use;
}

void CCamera::ProcessMouseMoveInput(float xoffset, float yoffset)
{
    if (m_UseFreeCam)
        MoveYawPitchFreeCam(xoffset, yoffset);
    else
        MoveCamSpherical(0.0025f * yoffset, 0.0025f * xoffset);
}

void CCamera::MoveFreeCam(ECamMoveDir dir, bool state)
{
    m_MoveDirKeyState[(uint)dir] = state;
}

void CCamera::MoveYawPitchFreeCam(float xoffset, float yoffset)
{
    xoffset *= 0.1f;
    yoffset *= 0.1f;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
  //  if (constrainPitch)
 //   {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
   // }

    // Update Front, Right and Up Vectors using the updated Euler angles

    // UpdateCameraVectors();
}

void CCamera::MoveCamSpherical(float psi, float fi)
{
    // vertical move
    m_SphereCamPSI -= psi;
    if (m_SphereCamPSI < -1.5f)
        m_SphereCamPSI = -1.5f;
    else if (m_SphereCamPSI > -0.1f)
        m_SphereCamPSI = -0.1f;

    // horizonthal move
    m_SphereCamFI -= fi;
}

void CCamera::ChangeViewSphereRadius(float r)
{
    m_SphereCamRadius += r;
    if (m_SphereCamRadius > 0)
        m_SphereCamRadius = 0;
}

void CCamera::UpdateCameraVectors()
{
    if (m_UseFreeCam)
        UpdateFreeCamVectors();
    else
        UpdateCamSphericalVectors();
}

void CCamera::UpdateCamSphericalVectors()
{
    m_View = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Eye.x = m_View.x + m_SphereCamRadius * cos(m_SphereCamPSI) * cos(m_SphereCamFI);
    m_Eye.y = m_View.y + m_SphereCamRadius * sin(m_SphereCamPSI);
    m_Eye.z = m_View.z + m_SphereCamRadius * cos(m_SphereCamPSI) * sin(m_SphereCamFI);
}

void CCamera::UpdateFreeCamVectors()
{
    UpdateFreeCamPos();
    UpdateFreeCamView();

    // Update right and up
    m_Right = glm::normalize(glm::cross(m_View, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_View));
}

void CCamera::UpdateFreeCamPos()
{
    if (m_MoveDirKeyState[(uint)ECamMoveDir::FORWARD])
        m_Eye += m_View * m_MoveSpeed;
    if (m_MoveDirKeyState[(uint)ECamMoveDir::BACKWARD])
        m_Eye -= m_View * m_MoveSpeed;
    if (m_MoveDirKeyState[(uint)ECamMoveDir::RIGHT])
        m_Eye += m_Right * m_MoveSpeed;
    if (m_MoveDirKeyState[(uint)ECamMoveDir::LEFT])
        m_Eye -= m_Right * m_MoveSpeed;
    if (m_MoveDirKeyState[(uint)ECamMoveDir::UP])
        m_Eye += m_Up * m_MoveSpeed;
    if (m_MoveDirKeyState[(uint)ECamMoveDir::DOWN])
        m_Eye -= m_Up * m_MoveSpeed;
}

void CCamera::UpdateFreeCamView()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_View = glm::normalize(front);
}