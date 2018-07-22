#include "stdafx.h"
#include "ITechnique.h"

ITechnique::~ITechnique()
{
}

bool ITechnique::Init()
{
    m_Renderer = g_Engine->Renderer();

    if (!CreateGraphicsPipeline())
        return Shutdown();

    return true;
}

bool ITechnique::Shutdown()
{
    if (m_Renderer)
    {
        if (m_GraphicsPipeline)
            vkDestroyPipeline(m_Renderer->GetDevice(), m_GraphicsPipeline, nullptr);

        if (m_PipelineLayout)
            vkDestroyPipelineLayout(m_Renderer->GetDevice(), m_PipelineLayout, nullptr);

        m_Renderer = nullptr;
    }

    return false;
}

bool ITechnique::CreateGraphicsPipeline()
{
    // Shaders
    SShaderMgrParams shaders_params;
    GetShadersDesc(shaders_params);
    CShaderManager shadersMgr(shaders_params);

    // Vertex input
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    GetVertexInputDesc(vertexInputInfo);

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    GetInputAssemblyDesc(inputAssembly);

    // Viewports and scissors
    VkPipelineViewportStateCreateInfo viewportState = {};
    GetViewportDesc(viewportState);

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    GetRasterizerDesc(rasterizer);

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    GetMultisamplingDesc(multisampling);

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    GetDepthStencilDesc(depthStencil);

    // Color blending
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    GetColorBlendDesc(colorBlending);

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    GetDynamicStateDesc(dynamicState);

    // Pipeline layout
    if (!CreatePipelineLayout())
        return false;

    // Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shadersMgr.GetShaderStageInfoVec().size();
    pipelineInfo.pStages = shadersMgr.GetShaderStageInfoVec().data(); //#SHADERS to zadziala przy multiple shaders?
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    //pipelineInfo.pDynamicState = nullptr; // &dynamicState; // Optional
    pipelineInfo.layout = m_PipelineLayout;
    pipelineInfo.renderPass = m_Renderer->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    //pipelineInfo.basePipelineIndex = -1; // Optional

    if (VKRESULT(vkCreateGraphicsPipelines(m_Renderer->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline)))
        return utils::FatalError(g_Engine->Hwnd(), "Failed to create graphics pipeline");

    return true;
}

size_t ITechnique::GetUniBuffObjOffset() const
{
    size_t minUboAlignment = g_Engine->Renderer()->MinUboAlignment();
    return (GetSingleUniBuffObjSize() + minUboAlignment - 1) & ~(minUboAlignment - 1);
}

void ITechnique::GetInputAssemblyDesc(VkPipelineInputAssemblyStateCreateInfo& inputAssembly)
{
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
}

void ITechnique::GetViewportDesc(VkPipelineViewportStateCreateInfo& viewportState)
{
    static VkViewport viewport = {}; // #TECH_UGH static
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_Renderer->GetSwapChainExtent().width;
    viewport.height = (float)m_Renderer->GetSwapChainExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    static VkRect2D scissor = {}; // #TECH_UGH static
    scissor.offset = { 0, 0 };
    scissor.extent = m_Renderer->GetSwapChainExtent();

    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
}

void ITechnique::GetRasterizerDesc(VkPipelineRasterizationStateCreateInfo& rasterizer)
{
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;;

    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
}

void ITechnique::GetMultisamplingDesc(VkPipelineMultisampleStateCreateInfo& multisampling)
{
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
}

void ITechnique::GetDepthStencilDesc(VkPipelineDepthStencilStateCreateInfo& depthStencil)
{
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional
}

void ITechnique::GetColorBlendDesc(VkPipelineColorBlendStateCreateInfo& colorBlending)
{
    // Color blending Attachment
    static VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAttachment.blendEnable = VK_FALSE;
    // colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    // colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    // Color blending
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_TRUE; //VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional
}

void ITechnique::GetDynamicStateDesc(VkPipelineDynamicStateCreateInfo& dynamicState)
{
    // VkDynamicState dynamicStates = {};
    // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicState.dynamicStateCount = 0;
    // dynamicState.pDynamicStates = nullptr;

    // A limited amount of the state can actually be changed without recreating the pipeline. 
    // Examples are the size of the viewport, line width and blend constants. 
    // If you want to do that, then you'll have to fill in a VkPipelineDynamicStateCreateInfo structure like this.
    // This will cause the configuration of these values to be ignored and you will be required to specify the data at drawing time.

    static VkDynamicState dynamicStates[] = // #TECH_UGH...
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
}

void ITechnique::GetPipelineLayoutDesc(VkPipelineLayoutCreateInfo& pipelineLayoutInfo)
{
    // OLD
    // pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // pipelineLayoutInfo.setLayoutCount = 0; // Optional
    // pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    // pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    // pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    // #UNI_BUFF tutaj wazne oooo
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &g_Engine->Renderer()->m_DescriptorSetLayout;
}

bool ITechnique::CreatePipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    GetPipelineLayoutDesc(pipelineLayoutInfo);

    if (VKRESULT(vkCreatePipelineLayout(g_Engine->Renderer()->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout)))
        return utils::FatalError(g_Engine->Hwnd(), "Failed to create pipeline layout");

    return true;
}