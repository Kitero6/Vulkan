#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <vector>
#include <optional>
#include <string>

#include "VulkanIncludes.h"
#include "Vertex.h"

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

    struct UniformBufferObject
    {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 projection;
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

        const std::vector<Vertex> vertices {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
        };

        const std::vector<uint16_t> indices {
            0, 1, 2, 2, 3, 0
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
        VkDescriptorSetLayout _descriptorSetLayout;
        VkPipelineLayout _pipelineLayout;
        VkPipeline       _graphicsPipeline;

        std::vector<VkFramebuffer> _swapChainFramebuffers;

        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer> _commandBuffers;

        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;
        VkBuffer _indexBuffer;
        VkDeviceMemory _indexBufferMemory;
        VkImage _textureImage;
        VkDeviceMemory _textureImageMemory;
        VkImageView _textureImageView;
        VkSampler _textureSampler;

        VkDescriptorPool _descriptorPool;
        std::vector<VkDescriptorSet> _descriptorSets;

        std::vector<VkBuffer> _uniformBuffers;
        std::vector<VkDeviceMemory> _uniformBuffersMemory;

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

        // ==== Descriptor Set Layout ==== //
        void CreateDescriptorSetLayout();

        // ==== Graphics Pipeline ==== //
        void CreateGraphicsPipeline();
        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        // ==== Render Pass ==== //
        void CreateRenderPass();

        // ==== Framebuffers ==== //
        void CreateFramebuffers();

        // ==== Command Pool ==== //
        void CreateCommandPool();

        // ==== Texture Image ==== //
        void CreateTextureImage();
        void CreateImage(
            uint32_t width, 
            uint32_t height,
            VkFormat format,
            VkImageTiling tiling,
            VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkImage& image,
            VkDeviceMemory& imageMemory);
        void TransitionImageLayout(
            VkImage image, 
            VkFormat format, 
            VkImageLayout oldLayout, 
            VkImageLayout newLayout);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void CreateTextureImageView();
        void CreateTextureSampler();

        VkImageView CreateImageView(VkImage image, VkFormat format) ;


        // ==== Descriptor Sets ==== //
        void CreateDescriptorSets();

        // ==== Buffers ==== //
        // ==== Vertex Buffer ==== //
        void CreateVertexBuffer();

        // ==== Index Buffer ==== //
        void CreateIndexBuffer();

        // ==== Uniform Buffer ==== //
        void CreateUniformBuffer();

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        // ==== Descriptor Pool ==== //
        void CreateDescriptorPool();

        // ==== Command Buffers ==== //
        void CreateCommandBuffers();
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

        // ==== Semaphores ==== //
        void CreateSyncObjects();
        #pragma endregion //Initialization

        void RecreateSwapChain();
        void CleanupSwapChain();

        #pragma region MainLoop
        void MainLoop();

        void DrawFrame();
        void UpdateUniformBuffer(uint32_t currentImage);
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