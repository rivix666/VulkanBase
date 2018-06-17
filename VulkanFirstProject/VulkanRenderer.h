#pragma once
#include "IGObject.h"

class CTechniqueManager;

//#UNI_BUFF podzielic to na oddzielny dla cam co bedzie staly i oddzielny per object
struct UniformBufferObject 
{
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 obj_world;
};

class CVulkanRenderer
{
public:
    CVulkanRenderer(GLFWwindow* window);
    ~CVulkanRenderer();

    // Init
    bool Init();
    bool Shutdown();

    // Render
    void Render();

    // Getters
    VkDevice GetDevice() const { return m_Device; }
    VkRenderPass GetRenderPass() const { return m_RenderPass; }
    CTechniqueManager* GetTechMgr() const { return m_TechMgr; }

    const VkExtent2D& GetSwapChainExtent() const { return m_SwapChainExtent; }

    // Buffers
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    bool CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

    // Misc
    void PresentQueueWaitIdle();
    void RecreateCommandBuffer(); //#CMD_BUFF nie dziala w ogole potrzebne??

protected:
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices
    {
        int graphicsFamily = -1;
        int presentFamily = -1;
        bool IsComplete()
        {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

#ifdef _DEBUG
    // Debug
    bool CheckValidationLayerSupport();
    void DestroyDebugReportCallbackEXT(VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
    VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
#endif

    // Init Vulkan
    bool InitVkInstance();
    bool InitWindowSurface();
    bool CreateSwapChain();
    bool CreateImageViews();
    bool CreateLogicalDevice();
    bool PickPhysicalDevice();
    bool CreateRenderPass();
    bool CreateFramebuffers();
    bool CreateCommandPool();
    bool CreateCommandBuffers();
    bool CreateSemaphores();

    bool InitTechniqueManager();

    // Extensions support
    bool CheckAvailableExtensions();
    bool GetRequiredExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    int  RateDeviceSuitability(VkPhysicalDevice device);

    // SwapChain support
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    // SwapChain recreation (after resize)
    //#CMD_BUFF
public:
    void RecreateSwapChain();
protected:


    void CleanupSwapChain();
    bool RecreateSwapChainIfNeeded(const VkResult& result, bool allow_suboptimal = true);

    //#UNI_BUFF
    //////////////////////////////////////////////////////////////////////////
public:
    bool CreateDescriptorSetLayout();

    VkDescriptorSetLayout m_DescriptorSetLayout = nullptr;


    bool CreateUniformBuffer();

    VkBuffer m_UniformBuffer = nullptr;
    VkDeviceMemory m_UniformBufferMemory = nullptr;

    bool CreateDescriptorPool();

    VkDescriptorPool m_DescriptorPool = nullptr;

    bool CreateDescriptorSet();

    VkDescriptorSet m_DescriptorSet = nullptr;

protected:
    //////////////////////////////////////////////////////////////////////////


    //#IMAGES
    //////////////////////////////////////////////////////////////////////////

    // part one
    //////////////////////////////////////////////////////////////////////////
    bool CreateTextureImage();
    bool CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    // U¿ywane w buffers copy
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    uint32_t m_MipLevels;
    VkImage m_TextureImage = nullptr;
    VkDeviceMemory m_TextureImageMemory = nullptr;
    //////////////////////////////////////////////////////////////////////////

    bool CreateTextureImageView();
    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels); //#TYPEDEF_UINT zamienic wszystkie moje uinty na uint32_t

    VkImageView m_TextureImageView = nullptr;
    VkSampler m_TextureSampler = nullptr;

    bool CreateTextureSampler();

    // #MIPMAPS
    //////////////////////////////////////////////////////////////////////////
    void GenerateMipmaps(VkImage image, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
    //////////////////////////////////////////////////////////////////////////


    // #DEPTH
    //////////////////////////////////////////////////////////////////////////
    bool CreateDepthResources();

    VkFormat FindDepthFormat();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    bool HasStencilComponent(VkFormat format);

    VkImage m_DepthImage = nullptr;
    VkDeviceMemory m_DepthImageMemory = nullptr;
    VkImageView m_DepthImageView = nullptr;
    //////////////////////////////////////////////////////////////////////////


    // Render
    bool SubmitDrawCommands(const uint32_t& imageIndex, VkSubmitInfo& submitInfo);

    // Pipeline cache
    // bool CreatePipelineCache();

private:
    // Window
    GLFWwindow*         m_Window = nullptr;

    // Vulkan handle
    VkInstance          m_Instance = nullptr;       // Vulkan instance
    VkSurfaceKHR        m_Surface = nullptr;        // Window surface where the view will be rendered
    VkDevice            m_Device = nullptr;         // Logical device handle
    VkPhysicalDevice    m_PhysicalDevice = nullptr; // Physical device handle
    VkQueue             m_GraphicsQueue = nullptr;  // Render queue
    VkQueue             m_PresentQueue = nullptr;   // Presentation queue

    // Pipeline handle
    VkRenderPass        m_RenderPass = nullptr;
    CTechniqueManager*  m_TechMgr = nullptr;

    // SwapChain handle
    VkSwapchainKHR      m_SwapChain = nullptr;
    VkFormat            m_SwapChainImageFormat;
    VkExtent2D          m_SwapChainExtent;

    std::vector<VkImage> m_SwapChainImagesVec;
    std::vector<VkImageView> m_SwapChainImageViewsVec;
    std::vector<VkFramebuffer> m_SwapChainFramebuffersVec;

    // Command buffers
    VkCommandPool m_CommandPool = nullptr;
    std::vector<VkCommandBuffer> m_CommandBuffers;

    // Swap chain events synchronization
    VkSemaphore m_ImageAvailableSemaphore = nullptr;
    VkSemaphore m_RenderFinishedSemaphore = nullptr;

    // Misc
    bool m_EnableTripleBuffering = false;

    // Required device extensions vector. Without them application won't start
    const std::vector<const char*> m_ReqDeviceExt = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; 

    // Required instance extensions
    uint32_t m_InstanceExtCount = 0;
    std::vector<const char*> m_ReqInstanceExt;

    // Validation layers
    const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_LUNARG_standard_validation" };

    // Render objects
    std::vector<IGObject*> m_Objects;

#ifdef _DEBUG
    // Debug
    VkDebugReportCallbackEXT m_DebugCallback = nullptr;
#endif
};

