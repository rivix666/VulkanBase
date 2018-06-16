#include "stdafx.h"
#include "GBaseObject.h"

uint CGBaseObject::s_TechId = UINT_MAX;

CGBaseObject::CGBaseObject()
{
    m_Vertices = 
    {
        { { -10000.0f, 2.5f, -10000.0f }, { 0.0f, 0.0f, 1.0f } },
        { {  10000.0f, 2.5f, -10000.0f }, { 0.0f, 1.0f, 0.0f } },
        { {  10000.0f, 2.5f,  10000.0f }, { 1.0f, 0.0f, 0.0f } },
        { { -10000.0f, 2.5f,  10000.0f }, { 0.0f, 1.0f, 0.0f } },
    };

    m_Indices = {
        0, 1, 2, 2, 3, 0
    };

    m_TechId = s_TechId;
}

bool CGBaseObject::CreateBuffers()
{
    if (!m_VertexBuffer)
        CreateVertexBuffer();

    if (!m_IndexBuffer)
        CreateIndexBuffer();

    return true;
}

size_t CGBaseObject::GetVertexSize() const
{
    return sizeof(BaseVertex);
}

size_t CGBaseObject::GetVerticesSize() const
{
    return GetVertexSize() * m_Vertices.size();
}

size_t CGBaseObject::GetIndexSize() const
{
    return sizeof(uint16_t);
}

size_t CGBaseObject::GetIndicesSize() const
{
    return GetIndexSize() * m_Indices.size();
}

uint CGBaseObject::GetIndicesCount() const
{
    return m_Indices.size();
}

uint CGBaseObject::GetVerticesCount() const
{
    return m_Vertices.size();
}

void* CGBaseObject::GetVerticesPtr() 
{
    return &m_Vertices[0];
}

void* CGBaseObject::GetIndicesPtr()
{
    return &m_Indices[0];
}

void CGBaseObject::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = GetVerticesSize();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    g_Engine->GetRenderer()->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(g_Engine->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, GetVerticesPtr(), (size_t)bufferSize);
    vkUnmapMemory(g_Engine->GetDevice(), stagingBufferMemory);

    g_Engine->GetRenderer()->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

    g_Engine->GetRenderer()->CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

    vkDestroyBuffer(g_Engine->GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(g_Engine->GetDevice(), stagingBufferMemory, nullptr);
}

void CGBaseObject::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = GetIndicesSize();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    g_Engine->GetRenderer()->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(g_Engine->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, GetIndicesPtr(), (size_t)bufferSize);
    vkUnmapMemory(g_Engine->GetDevice(), stagingBufferMemory);

    g_Engine->GetRenderer()->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

    g_Engine->GetRenderer()->CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

    vkDestroyBuffer(g_Engine->GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(g_Engine->GetDevice(), stagingBufferMemory, nullptr);
}