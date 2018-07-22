#include "stdafx.h"
#include "GObjectControl.h"
#include "TechniqueManager.h"
#include "IGObject.h"

#include "GBaseObject.h" //#UNI_BUFF

CGObjectControl::CGObjectControl(VkDevice device)
    : m_Device(device)
{
    uint tech_count = g_Engine->Renderer()->GetTechMgr()->TechniquesCount();
    m_TechToObjVec.resize(tech_count);
    m_SizeCacheVec.resize(tech_count);
}

CGObjectControl::~CGObjectControl()
{
}

void CGObjectControl::Shutdown()
{
    uint tech_count = m_TechToObjVec.size();
    for (uint i = 0; i < tech_count; i++)
    {
        for (auto obj : m_TechToObjVec[i])
            SAFE_DELETE(obj);
    }

    m_TechToObjVec.clear();
}

void CGObjectControl::RegisterObject(IGObject* obj)
{
    if (!obj)
        return;

    if (m_TechToObjVec[obj->TechniqueId()].size() >= OBJ_PER_TECHNIQUE)
    {
        utils::FatalError(g_Engine->Hwnd(), "Can not register more objects");
        return;
    }

    obj->CreateBuffers();
    EnsureTechIdWillFit(obj->TechniqueId());

    m_TechToObjVec[obj->TechniqueId()].push_back(obj);
    m_SizeCacheVec[obj->TechniqueId()] += obj->GetVerticesSize();
}

void CGObjectControl::RegisterObject(const uint& tech, IGObject* obj)
{
    if (!obj)
        return;

    if (m_TechToObjVec[tech].size() >= OBJ_PER_TECHNIQUE)
    {
        utils::FatalError(g_Engine->Hwnd(), "Can not register more objects");
        return;
    }

    obj->CreateBuffers();
    EnsureTechIdWillFit(tech);

    m_TechToObjVec[tech].push_back(obj);
    m_SizeCacheVec[tech] += obj->GetVerticesSize();
}

void CGObjectControl::UnregisterObject(const uint& tech, IGObject* obj)
{
    if (tech >= m_TechToObjVec.size() || !obj)
        return;

    m_SizeCacheVec[tech] -= obj->GetVerticesSize();
    std::remove_if(m_TechToObjVec[tech].begin(), m_TechToObjVec[tech].end(), [=](IGObject* o) { return o == obj; });
}

void CGObjectControl::RecordCommandBuffer(VkCommandBuffer& cmd_buff)
{
    auto tech_mgr = g_Engine->Renderer()->GetTechMgr();
    uint tech_count = m_TechToObjVec.size();
    for (uint i = 0; i < tech_count; i++)
    {
        auto tech = tech_mgr->GetTechnique(i);
        vkCmdBindPipeline(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipeline());

        for (uint j = 0; j < m_TechToObjVec[i].size(); j++)
        {
            auto obj = m_TechToObjVec[i][j];
            if (!obj->VertexBuffer())
                continue;

            // Bind Vertex buffer
            VkBuffer vertexBuffers[] = { obj->VertexBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(cmd_buff, 0, 1, vertexBuffers, offsets);

            // Prepare uni buff offset
            uint32_t uni_offset = tech->GetUniBuffObjOffset() * j;

            // Record command
            if (obj->IndexBuffer())
            {
                vkCmdBindIndexBuffer(cmd_buff, obj->IndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
                vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipelineLayout(), 0, 1, &g_Engine->Renderer()->m_DescriptorSet, 1, &uni_offset);  //#UNI_BUFF
                vkCmdDrawIndexed(cmd_buff, static_cast<uint32_t>(obj->GetIndicesCount()), 1, 0, 0, 0);
            }
            else
            {
                vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipelineLayout(), 0, 1, &g_Engine->Renderer()->m_DescriptorSet, 1, &uni_offset);  //#UNI_BUFF
                vkCmdDraw(cmd_buff, static_cast<uint32_t>(obj->GetVerticesCount()), 1, 0, 0);
            }
        }
    }
}

//#UNI_BUFF
/*
wyliczac vector offsetow od razu per technika i rejestracja obiektow, posprzatac wsio
Good Luck
*/
void CGObjectControl::UpdateUniBuffers()
{
    size_t minUboAlignment = g_Engine->Renderer()->MinUboAlignment();

    // Update Uni buffers
    for (uint tech_id = 0; tech_id < m_TechToObjVec.size(); tech_id++)
    {
        uint8_t* pData;
        auto uni_buff_mem = g_Engine->Renderer()->GetTechMgr()->GetTechnique(tech_id)->BaseObjUniBufferMemory();
        auto single_obj_size = g_Engine->Renderer()->GetTechMgr()->GetTechnique(tech_id)->GetSingleUniBuffObjSize();
        vkMapMemory(g_Engine->Device(), uni_buff_mem, 0, single_obj_size * m_TechToObjVec[tech_id].size(), 0, (void **)&pData);
        for (uint obj_id = 0; obj_id < m_TechToObjVec[tech_id].size(); obj_id++)
        {
            void* obj_data = m_TechToObjVec[tech_id][obj_id]->GetUniBuffData();
            if (obj_data)
            {
                memcpy(pData, obj_data, single_obj_size);
            }
            else
            {
                LogD("Object with invalid UniBuff data. Tech:");
                LogD(tech_id);
                LogD(" Obj:");
                LogD(obj_id);
                LogD("\n");
            }
            pData += g_Engine->Renderer()->GetTechMgr()->GetTechnique(tech_id)->GetUniBuffObjOffset();
        }
        vkUnmapMemory(g_Engine->Device(), uni_buff_mem);
    }
}

void CGObjectControl::EnsureTechIdWillFit(const uint& tech_id)
{
    if (m_TechToObjVec.size() <= tech_id)
    {
        m_TechToObjVec.resize(tech_id);
        m_SizeCacheVec.resize(tech_id);
    }
}