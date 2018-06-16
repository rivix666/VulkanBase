#include "stdafx.h"
#include "BaseTechnique.h"
#include "ShaderManager.h"

VkVertexInputBindingDescription BaseVertex::m_BindingDesc = {};
std::array<VkVertexInputAttributeDescription, 2> BaseVertex::m_AttributeDesc = {};

VkVertexInputBindingDescription* BaseVertex::GetBindingDescription()
{
    static bool prepared = false; // #TECH_UGH...
    if (!prepared)
    {
        m_BindingDesc.binding = 0;
        m_BindingDesc.stride = sizeof(BaseVertex);
        m_BindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        prepared = true;
    }
    return &m_BindingDesc;
}

std::array<VkVertexInputAttributeDescription, 2>* BaseVertex::GetAttributeDescriptions()
{
    static bool prepared = false; // #TECH_UGH... // moze szykowac je jak graphics pipeline czyli przychodza tutaj z gory
    if (!prepared)
    {
        m_AttributeDesc[0].binding = 0;
        m_AttributeDesc[0].location = 0;
        m_AttributeDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT; //#INPUT_VERTEX_WAZNE pamietaj ot ym gonwie
        m_AttributeDesc[0].offset = offsetof(BaseVertex, pos);

        m_AttributeDesc[1].binding = 0;
        m_AttributeDesc[1].location = 1;

        m_AttributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        m_AttributeDesc[1].offset = offsetof(BaseVertex, color);
        prepared = true;
    }
    return &m_AttributeDesc;
}

CBaseTechnique::CBaseTechnique()
    : ITechnique()
{

}

CBaseTechnique::~CBaseTechnique()
{
}

void CBaseTechnique::GetVertexInputDesc(VkPipelineVertexInputStateCreateInfo& vertexInputInfo)
{
    auto bindingDescription = BaseVertex::GetBindingDescription();
    auto attributeDescriptions = BaseVertex::GetAttributeDescriptions();
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions->size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions->data();
}

void CBaseTechnique::GetShadersDesc(SShaderMgrParams& params)
{
    params.vertex_shader_path = "Effects/vert.spv";
    params.fragment_shader_path = "Effects/frag.spv";
    params.vertex_entry = "main";
    params.fragment_entry = "main";
}
