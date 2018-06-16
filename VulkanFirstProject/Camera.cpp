#include "stdafx.h"
#include "Camera.h"

void CCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float zNear, float zFar)
{
    FOV = FOV * (float)DEG_TO_RAD;
    m_ProjectionMtx = glm::perspective(FOV, aspectRatio, zNear, zFar);
    m_InvProjectionMtx = glm::inverse(m_ProjectionMtx);
}

void CCamera::Update()
{
    //#SPHERE_CAM
     //MoveCamSpherical();




     //#FREE_CAMERA

//     glm::mat4 rotation_mtx = glm::rotate(glm::mat4(1.0f), m_Heading, glm::vec3(1.0f, 0.0f, 0.0f));
//     rotation_mtx = glm::rotate(rotation_mtx, m_Pitch, glm::vec3(0.0f, 0.0f, 1.0f));
// 
//     D3DXMatrixRotationYawPitchRoll(&rotationMatrix, heading, pitch, 0);
// 
//     D3DXVec3TransformCoord(&view, &dV, &rotationMatrix);
//     D3DXVec3TransformCoord(&up, &dU, &rotationMatrix);
// 
//     D3DXVec3Normalize(&forward, &view);
//     D3DXVec3Cross(&strafeRight, &up, &view);
//     D3DXVec3Normalize(&strafeRight, &strafeRight);
// 
//     view = eye + view;



     m_ViewMtx = glm::lookAt(m_Eye, m_View, m_Up);
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

void CCamera::AdjustHeadingPitch(float hRad, float pRad)
{
    m_Heading += hRad;
    m_Pitch += pRad;

    // Keep heading and pitch betweem 0 and 2pi
    if (m_Heading > TWO_PI) m_Heading -= (float)TWO_PI;
    else if (m_Heading < 0) m_Heading = (float)TWO_PI + m_Heading;

    if (m_Pitch > TWO_PI) m_Pitch -= (float)TWO_PI;
    else if (m_Pitch < 0) m_Pitch = (float)TWO_PI + m_Pitch;
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