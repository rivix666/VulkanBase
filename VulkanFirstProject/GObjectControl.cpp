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

    obj->CreateBuffers();
    EnsureTechIdWillFit(obj->TechniqueId());

    m_TechToObjVec[obj->TechniqueId()].push_back(obj);
    m_SizeCacheVec[obj->TechniqueId()] += obj->GetVerticesSize();
}

void CGObjectControl::RegisterObject(const uint& tech, IGObject* obj)
{
    if (!obj)
        return;

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
    auto tech_mgr = g_Engine->GetRenderer()->GetTechMgr();
    uint tech_count = m_TechToObjVec.size();
    for (uint i = 0; i < tech_count; i++)
    {
        auto tech = tech_mgr->GetTechnique(i);
        vkCmdBindPipeline(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipeline());

        //#BUFFERS zrobic to jakos madrze kiedys, pogrupowac, ponadawac offsety, by nie leciec tu w drugiej petli tylko za jednym razem machanac wsio
        for (auto obj : m_TechToObjVec[i])
        {
            if (!obj->VertexBuffer())
                continue;

            VkBuffer vertexBuffers[] = { obj->VertexBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(cmd_buff, 0, 1, vertexBuffers, offsets);

            if (obj->IndexBuffer())
            {
                vkCmdBindIndexBuffer(cmd_buff, obj->IndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

                //#NDIUWJND
                vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipelineLayout(), 0, 1, &g_Engine->GetRenderer()->m_DescriptorSet, 0, nullptr);  //#UNI_BUFF
                vkCmdDrawIndexed(cmd_buff, static_cast<uint32_t>(obj->GetIndicesCount()), 1, 0, 0, 0);
            }
            else
            {

                //#NDIUWJND
                vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipelineLayout(), 0, 1, &g_Engine->GetRenderer()->m_DescriptorSet, 0, nullptr);  //#UNI_BUFF
                vkCmdDraw(cmd_buff, static_cast<uint32_t>(obj->GetVerticesCount()), 1, 0, 0);
            }
        }
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