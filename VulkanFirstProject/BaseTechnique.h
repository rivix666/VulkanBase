#pragma once
#include "ITechnique.h"

struct BaseVertex
{
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription m_BindingDesc;
    static std::array<VkVertexInputAttributeDescription, 2> m_AttributeDesc;

    static VkVertexInputBindingDescription* GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2>* GetAttributeDescriptions();
};

class CBaseTechnique : public ITechnique
{
public:
    CBaseTechnique();
    ~CBaseTechnique();

protected:
    void GetVertexInputDesc(VkPipelineVertexInputStateCreateInfo& vertexInputInfo) override;
    void GetShadersDesc(SShaderMgrParams& params) override;
};

