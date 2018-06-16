#include "stdafx.h"
#include "CCamera.h"

void CCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float zNear, float zFar)
{
    FOV = FOV * (float)DEG_TO_RAD;
    m_ProjectionMtx = glm::perspective(FOV, aspectRatio, zNear, zFar);
    m_InvProjectionMtx = glm::inverse(m_ProjectionMtx);
}

void CCamera::Update()
{
    MoveCamSpherical();
    m_ViewMtx = glm::lookAtLH(m_Eye, m_View, m_Up);
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

void CCamera::MoveCamSpherical()
{
    glm::vec3 temp(0.0f, 0.0f, 0.0f); // view
    m_Eye.x = temp.x + m_SphereCamRadius * cos(m_SphereCamPSI) * cos(m_SphereCamFI);
    m_Eye.y = temp.y + m_SphereCamRadius * sin(m_SphereCamPSI);
    m_Eye.z = temp.z + m_SphereCamRadius * cos(m_SphereCamPSI) * sin(m_SphereCamFI);
}

void CCamera::ChangeViewSphereRadius(float r)
{
    m_SphereCamRadius += r;
    if (m_SphereCamRadius > 0)
        m_SphereCamRadius = 0;
}

void CCamera::AddToView(float x, float y, float z)
{
    m_View.x += x;
    m_View.y += y;
    m_View.z += z;
}

void CCamera::AddToEye(float x, float y, float z)
{
    m_Eye.x += x;
    m_Eye.y += y;
    m_Eye.z += z;
}