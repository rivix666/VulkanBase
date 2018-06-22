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
    //size_t minUboAlignment = g_Engine->Device()->properties.limits.minUniformBufferOffsetAlignment;

    // Calculate required alignment based on minimum device offset alignment
    //             dynamicAlignment = sizeof(glm::mat4);
    //             if (minUboAlignment > 0) {
    //                 dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
    //             }

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(g_Engine->Renderer()->GetPhysicalDevice(), &props);


    size_t minUboAlignment = props.limits.minUniformBufferOffsetAlignment;

    uint32_t offsets2[2];
    offsets2[0] = 0;
    offsets2[1] = sizeof(SObjUniBuffer);

    if (minUboAlignment > 0)
    {
        offsets2[0] = (offsets2[0] + minUboAlignment - 1) & ~(minUboAlignment - 1);
        offsets2[1] = (offsets2[1] + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }

    // Update Uni buffer
    //obj->UpdateUniformBuffer(g_Engine->Renderer()->BaseObjUniBufferMemory());

    SObjUniBuffer ub = {};
    ub.obj_world = m_TechToObjVec.front()[0]->WorldMtx();
    ub.tex_mul = m_TechToObjVec.front()[0]->m_TexMultiplier;

    SObjUniBuffer ub2 = {};
    ub2.obj_world = m_TechToObjVec.front()[1]->WorldMtx();
    ub2.tex_mul = m_TechToObjVec.front()[1]->m_TexMultiplier;

    uint8_t *pData;
    vkMapMemory(g_Engine->Device(), g_Engine->Renderer()->BaseObjUniBufferMemory(), 0, sizeof(SObjUniBuffer) * 2, 0, (void **)&pData);
    memcpy(pData, &ub, sizeof(ub));
    pData += (sizeof(SObjUniBuffer) + minUboAlignment - 1) & ~(minUboAlignment - 1);//sizeof(ub); //#UNI_BUFF
    memcpy(pData, &ub2, sizeof(ub2));
    vkUnmapMemory(g_Engine->Device(), g_Engine->Renderer()->BaseObjUniBufferMemory());
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    auto tech_mgr = g_Engine->Renderer()->GetTechMgr();
    uint tech_count = m_TechToObjVec.size();
    for (uint i = 0; i < tech_count; i++)
    {
        auto tech = tech_mgr->GetTechnique(i);
        vkCmdBindPipeline(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipeline());

        int kaka = 0;
        //#BUFFERS zrobic to jakos madrze kiedys, pogrupowac, ponadawac offsety, by nie leciec tu w drugiej petli tylko za jednym razem machanac wsio
        for (auto obj : m_TechToObjVec[i])
        {
            if (!obj->VertexBuffer())
                continue;

            // Bind Vertex buffer
            VkBuffer vertexBuffers[] = { obj->VertexBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(cmd_buff, 0, 1, vertexBuffers, offsets);

            

            if (kaka == 0)
                offsets2[0] = 0;//(offsets2[0] + minUboAlignment - 1) & ~(minUboAlignment - 1);
            else
                offsets2[0] = (sizeof(SObjUniBuffer) + minUboAlignment - 1) & ~(minUboAlignment - 1);

            kaka++;

          //  VkPhysicalDeviceLimits::minUniformBufferOffsetAlignment
            if (obj->IndexBuffer())
            {
                vkCmdBindIndexBuffer(cmd_buff, obj->IndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
                vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipelineLayout(), 0, 1, &g_Engine->Renderer()->m_DescriptorSet, 1, offsets2);  //#UNI_BUFF
                vkCmdDrawIndexed(cmd_buff, static_cast<uint32_t>(obj->GetIndicesCount()), 1, 0, 0, 0);
            }
            else
            {
                vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, tech->GetPipelineLayout(), 0, 1, &g_Engine->Renderer()->m_DescriptorSet, 1, offsets2);  //#UNI_BUFF
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