#include "stdafx.h"
#include "GBaseObject.h"

uint CGBaseObject::s_TechId = UINT_MAX;

CGBaseObject::CGBaseObject()
{
    m_Vertices = 
    {
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.f } },
        { { 0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { { 0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f } },
        { { -0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
    };

    m_TechId = s_TechId;
}

size_t CGBaseObject::GetVertexSize() const
{
    return sizeof(BaseVertex);
}

size_t CGBaseObject::GetVerticesSize() const
{
    return GetVertexSize() * m_Vertices.size();
}

uint CGBaseObject::GetVerticesCount() const
{
    return m_Vertices.size();
}

void* CGBaseObject::GetVerticesPtr() 
{
    return &m_Vertices[0];
}