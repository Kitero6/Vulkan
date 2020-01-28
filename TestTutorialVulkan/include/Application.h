#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <string>

#define PHYSICAL_DEVICE_CHOICE_FIRST_DEVICE
#define PHYSICAL_DEVICE_CHOICE_RATE_DEVICE

namespace Vulkan
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        inline bool IsComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class Application
    {
        static constexpr int WIDTH  { 800 };
        static constexpr int HEIGHT { 600 };
        static constexpr int MAX_FRAMES_IN_FLIGHT { 2 };

        // CONSTANTS //
        const std::vector<const char*> _validationLayers
        {
            "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> _deviceExtensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        #ifdef NDEBUG
            const bool _enableValidationLayers { false };
        #else
            const bool _enableValidationLayers { true };
        #endif

    private:
        GLFWwindow* _window;
        VkInstance  _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;

        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice         _device;

        VkQueue _graphicsQueue;
        VkQueue _presentQueue;

        VkSurfaceKHR _surface;

        VkSwapchainKHR       _swapChain;
        std::vector<VkImage> _swapChainImages;
        VkFormat             _swapChainImageFormat;
        VkExtent2D           _swapChainExtent;

        std::vector<VkImageView> _swapChainImageViews;

        VkRenderPass     _renderPass;
        VkPipelineLayout _pipelineLayout;
        VkPipeline       _graphicsPipeline;

        std::vector<VkFramebuffer> _swapChainFramebuffers;

        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer> _commandBuffers;

        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;
        std::vector<VkFence> _inFlightFences;
        std::vector<VkFence> _imagesInFlight;
        size_t _currentFrame = 0;

        bool _isFramebufferResized = false;

        #pragma region Initialization
        void InitWindow();
        void InitVulkan();

        // ==== Instance ==== //
        void CreateInstance();
        std::vector<const char*> GetRequiredExtensions();

        // ==== Validation Layer ==== //
        void        SetupDebugMessage();
        bool        CheckValidationLayerSupport();
        void        PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        VkResult    CreateDebugUtilsMessengerEXT(
                        VkInstance instance, 
                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                        const VkAllocationCallbacks* pAllocator, 
                        VkDebugUtilsMessengerEXT* pDebugMessenger);
        void        DestroyDebugUtilsMessengerEXT(
                        VkInstance instance, 
                        VkDebugUtilsMessengerEXT debugMessenger, 
                        const VkAllocationCallbacks* pAllocator);

        // ==== Surface ==== //
        void CreateSurface();

        // ==== Physical Device ==== //
        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        int  RateDeviceSuitability(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

        // ==== Logical Device ==== //
        void CreateLogicalDevice();
        void PopulateQueueFamilyIndices(
            std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
            const QueueFamilyIndices& indices);

        // ==== Swap Chain ==== //
        void CreateSwapChain();
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        // ==== Image View ==== //
        void CreateImageViews();

        // ==== Graphics Pipeline ==== //
        void CreateGraphicsPipeline();
        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        // ==== Render Pass ==== //
        void CreateRenderPass();

        // ==== Framebuffers ==== //
        void CreateFramebuffers();

        // ==== Command Pool ==== //
        void CreateCommandPool();

        // ==== Command Buffers ==== //
        void CreateCommandBuffers();

        // ==== Semaphores ==== //
        void CreateSyncObjects();
        #pragma endregion //Initialization

        void RecreateSwapChain();
        void CleanupSwapChain();

        #pragma region MainLoop
        void MainLoop();

        void DrawFrame();
        #pragma endregion //MainLoop

        #pragma region Cleanup
        void Cleanup();
        #pragma endregion //Cleanup

    public:
        void Run();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        static std::vector<char> ReadFile(const std::string& filename);
        static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

        // ==== Accessors ==== //
        inline bool& IsFramebufferResized()       { return _isFramebufferResized; }
        inline bool  IsFramebufferResized() const { return _isFramebufferResized; }
    };
}

#endif// __Application_H__