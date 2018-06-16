#pragma once
#include "ITechnique.h"

class IGObject
{
public:
    IGObject() = default;
    ~IGObject();

    typedef unsigned int uint; // #TYPEDEF_UINT czemu nei bierze z stdafx??

    // Getters/Setters
    virtual size_t GetVertexSize() const = 0;
    virtual size_t GetVerticesSize() const = 0;
    virtual size_t GetIndexSize() const = 0;
    virtual size_t GetIndicesSize() const = 0;
    virtual uint   GetIndicesCount() const = 0;
    virtual uint   GetVerticesCount() const = 0;

    virtual void*  GetVerticesPtr() = 0;
    virtual void*  GetIndicesPtr() = 0;

    void SetTechId(uint tech_id) { m_TechId = tech_id; }
    uint GetTechniqueId() const { return m_TechId; }

    const VkBuffer& GetIndexBuffer() const { return m_IndexBuffer; }
    const VkBuffer& GetVertexBuffer() const { return m_VertexBuffer; }
    const VkDeviceMemory& GetIndexBufferMem() const { return m_IndexBufferMemory; }
    const VkDeviceMemory& GetVertexBufferMem() const { return m_VertexBufferMemory; }

    // Buffers handle
    virtual bool   CreateBuffers();
    virtual void   CleanupBuffers();

protected:
    uint m_TechId = UINT_MAX; //#TECH zobaczymy czy w ogole potrzebne

    // Buffers
    VkBuffer m_IndexBuffer = nullptr;
    VkBuffer m_VertexBuffer = nullptr;
    VkDeviceMemory m_IndexBufferMemory = nullptr;
    VkDeviceMemory m_VertexBufferMemory = nullptr;
};

