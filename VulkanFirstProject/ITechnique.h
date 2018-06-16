#pragma once
#include "ShaderManager.h"

class CVulkanRenderer;

class ITechnique
{
public:
    ITechnique() = default;
    virtual ~ITechnique();

    // Init
    bool Init();
    bool Shutdown();
    bool CreateGraphicsPipeline();

    // Getters
    VkPipeline GetPipeline() const              { return m_GraphicsPipeline; }
    VkPipelineLayout GetPipelineLayout() const  { return m_PipelineLayout; }

protected:
    // Pure virtual
    virtual void GetVertexInputDesc(VkPipelineVertexInputStateCreateInfo& vertexInputInfo) = 0;
    virtual void GetShadersDesc(SShaderMgrParams& params) = 0;

    // Get Pipeline description
    virtual void GetInputAssemblyDesc(VkPipelineInputAssemblyStateCreateInfo& inputAssembly);
    virtual void GetViewportDesc(VkPipelineViewportStateCreateInfo& viewportState);
    virtual void GetRasterizerDesc(VkPipelineRasterizationStateCreateInfo& rasterizer);
    virtual void GetMultisamplingDesc(VkPipelineMultisampleStateCreateInfo& multisampling);
    virtual void GetColorBlendDesc(VkPipelineColorBlendStateCreateInfo& colorBlending);
    virtual void GetDynamicStateDesc(VkPipelineDynamicStateCreateInfo& dynamicState);
    virtual void GetPipelineLayoutDesc(VkPipelineLayoutCreateInfo& pipelineLayoutInfo);

    // Create
    virtual bool CreatePipelineLayout();

    // Pipeline
    VkPipelineLayout m_PipelineLayout = nullptr;
    VkPipeline       m_GraphicsPipeline = nullptr;

    // Renderer
    CVulkanRenderer* m_Renderer = nullptr;
};
