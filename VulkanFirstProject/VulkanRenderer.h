#pragma once
#include "IGObject.h"

class CTechniqueManager;

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

