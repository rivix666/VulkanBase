#include "stdafx.h"
#include "GObjectControl.h"
#include "TechniqueManager.h"
#include "IGObject.h"

CGObjectControl::CGObjectControl(VkDevice device)
    : m_Device(device)
{
    uint tech_count = g_Engine->GetRenderer()->GetTechMgr()->TechniquesCount();
    m_TechToObjVec.resize(tech_count);
    m_SizeCacheVec.resize(tech_count);
    m_VertexBuffers.resize(tech_count, nullptr);
    m_IndexBuffers.resize(tech_count, nullptr);
    m_VertexBuffersMemory.resize(tech_count, nullptr);
    m_IndexBuffersMemory.resize(tech_count, nullptr);
}

CGObjectControl::~CGObjectControl()
{
}

void CGObjectControl::Shutdown()
{
    uint tech_count = m_TechToObjVec.size();
    for (uint i = 0; i < tech_count; i++)
    {
        CleanupBuffers(i);
        for (auto obj : m_TechToObjVec[i])
            SAFE_DELETE(obj);
    }

    m_VertexBuffers.clear();
    m_IndexBuffers.clear();
    m_VertexBuffersMemory.clear();
    m_IndexBuffersMemory.clear();
    m_TechToObjVec.clear();
}

void CGObjectControl::RegisterObject(IGObject* obj)
{
    if (!obj)
        return;

    EnsureTechIdWillFit(obj->GetTechniqueId());

    m_TechToObjVec[obj->GetTechniqueId()].push_back(obj);
    m_SizeCacheVec[obj->GetTechniqueId()] += obj->GetVerticesSize();

    // #VERTEX_BUFFERS czy konieczne?
    CleanupBuffers(obj->GetTechniqueId());
    RecreateBuffers(obj->GetTechniqueId());
}

void CGObjectControl::RegisterObject(const uint& tech, IGObject* obj)
{
    if (!obj)
        return;

    EnsureTechIdWillFit(tech);

    m_TechToObjVec[tech].push_back(obj);
    m_SizeCacheVec[tech] += obj->GetVerticesSize();

    // #VERTEX_BUFFERS czy konieczne?
    CleanupBuffers(tech);
    RecreateBuffers(tech);
}

void CGObjectControl::UnregisterObject(const uint& tech, IGObject* obj)
{
    if (tech >= m_TechToObjVec.size() || !obj)
        return;

    m_SizeCacheVec[tech] -= obj->GetVerticesSize();
    std::remove_if(m_TechToObjVec[tech].begin(), m_TechToObjVec[tech].end(), [=](IGObject* o) { return o == obj; });

    // #VERTEX_BUFFERS czy konieczne?
    CleanupBuffers(tech);
    RecreateBuffers(tech);
}

void CGObjectControl::RecordCommandBuffer(VkCommandBuffer& cmd_buff)
{
    auto tech_mgr = g_Engine->GetRenderer()->GetTechMgr();
    uint tech_count = m_TechToObjVec.size(); 
    for (uint i = 0; i < tech_count; i++)
    {
        auto tech = tech_mgr->GetTechnique(i);
        vkCmdBindPipeline(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipeline());

        VkBuffer vertexBuffers[] = { m_VertexBuffers[i] }; //// #O_KURWA tyy tu mozna wrzucic wiele vertex bufferow zamiast tak kombinowac to niech kazdy boeikt sobie go tworzy i trzyma
        // a my tutaj bedziemy je wszystkie wpychac tylko
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(cmd_buff, 0, 1, vertexBuffers, offsets);

        vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipelineLayout(), 0, 1, &g_Engine->GetRenderer()->m_DescriptorSet, 0, nullptr);  //#UNI_BUFF

        vkCmdDraw(cmd_buff, static_cast<uint32_t>(m_SizeCacheVec[i]), 1, 0, 0);
    }
}

bool CGObjectControl::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (VKRESULT(vkCreateBuffer(g_Engine->GetDevice(), &bufferInfo, nullptr, &buffer)))
        return utils::FatalError(g_Engine->GetHwnd(), "Failed to create buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(g_Engine->GetDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = g_Engine->GetRenderer()->FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (VKRESULT(vkAllocateMemory(g_Engine->GetDevice(), &allocInfo, nullptr, &bufferMemory)))
        return utils::FatalError(g_Engine->GetHwnd(), "Failed to allocate buffer memory");

    vkBindBufferMemory(g_Engine->GetDevice(), buffer, bufferMemory, 0);
    return true;
}

void CGObjectControl::CleanupBuffers(const uint& tech)
{
    if (m_VertexBuffers[tech])
    {
        vkDestroyBuffer(m_Device, m_VertexBuffers[tech], nullptr);
    }

    if (m_VertexBuffersMemory[tech])
    {
        vkFreeMemory(m_Device, m_VertexBuffersMemory[tech], nullptr);
    }

    if (m_IndexBuffers[tech])
    {
        vkDestroyBuffer(m_Device, m_IndexBuffers[tech], nullptr);
    }

    if (m_IndexBuffersMemory[tech])
    {
        vkFreeMemory(m_Device, m_IndexBuffersMemory[tech], nullptr);
    }
}

void CGObjectControl::RecreateBuffers(const uint& tech)
{
    if (m_VertexBuffers[tech] || m_VertexBuffersMemory[tech])
        CleanupBuffers(tech);
        
    CreateVertexBuffer(tech);
    CreateIndexBuffed(tech);
}

bool CGObjectControl::CreateVertexBuffer(const uint& tech)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = m_SizeCacheVec[tech];
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (VKRESULT(vkCreateBuffer(m_Device, &bufferInfo, nullptr, &m_VertexBuffers[tech])))
        return utils::FatalError(g_Engine->GetHwnd(), "Failed to create vertex buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_Device, m_VertexBuffers[tech], &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = g_Engine->GetRenderer()->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (VKRESULT(vkAllocateMemory(m_Device, &allocInfo, nullptr, &m_VertexBuffersMemory[tech])))
        return utils::FatalError(g_Engine->GetHwnd(), "Failed to allocate vertex buffer memory");

    vkBindBufferMemory(m_Device, m_VertexBuffers[tech], m_VertexBuffersMemory[tech], 0);

    void* data;
    vkMapMemory(m_Device, m_VertexBuffersMemory[tech], 0, bufferInfo.size, 0, &data);
    size_t offset = 0;
    for (auto obj : m_TechToObjVec[tech])
    {
        auto tmp = (char*)data + offset;
        memcpy((void*)tmp, obj->GetVerticesPtr(), obj->GetVerticesSize()); // #VERTEX_BUFFERS sprawdzic czy w ogole dziala
        offset += obj->GetVerticesSize();
    }
    vkUnmapMemory(m_Device, m_VertexBuffersMemory[tech]);

    return true;
}

bool CGObjectControl::CreateIndexBuffed(const uint& tech)
{
//     VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
// 
//     VkBuffer stagingBuffer;
//     VkDeviceMemory stagingBufferMemory;
//     createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
// 
//     void* data;
//     vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
//     memcpy(data, indices.data(), (size_t)bufferSize);
//     vkUnmapMemory(device, stagingBufferMemory);
// 
//     createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
// 
//     copyBuffer(stagingBuffer, indexBuffer, bufferSize);
// 
//     vkDestroyBuffer(device, stagingBuffer, nullptr);
//     vkFreeMemory(device, stagingBufferMemory, nullptr);
    return false;
}

void CGObjectControl::EnsureTechIdWillFit(const uint& tech_id)
{
    if (m_TechToObjVec.size() <= tech_id)
    {
        m_TechToObjVec.resize(tech_id);
        m_SizeCacheVec.resize(tech_id);
        m_VertexBuffers.resize(tech_id, nullptr);
        m_IndexBuffers.resize(tech_id, nullptr);
        m_VertexBuffersMemory.resize(tech_id, nullptr);
        m_IndexBuffersMemory.resize(tech_id, nullptr);
    }
}