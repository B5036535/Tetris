#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <cstring>
#include <optional>
#include <vulkan\vulkan_core.h>

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};


class Game;
class Renderer
{
public:
	Renderer(GLFWwindow* w, Game* g, const uint32_t width, const uint32_t height) : WIDTH(width), HEIGHT(height)
	{

		window = w;
		game = g;
		InitVulkan();
	}
	~Renderer()
	{
		CleanUp();
	}

	void DrawFrame();

private:

	const uint32_t WIDTH;
	const uint32_t HEIGHT;

	GLFWwindow* window;
	Game* game;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;

	VkPipelineLayout pipelineLayout_block;
	VkPipelineLayout pipelineLayout_ball;
	VkPipelineLayout pipelineLayout_gameState;
	VkPipeline graphicsPipeline_block;
	VkPipeline graphicsPipeline_ball;
	VkPipeline graphicsPipeline_gameState;

	VkCommandPool commandPool;

	VkImage colourImage;
	VkDeviceMemory colourImageMemory;
	VkImageView colourImageView;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	enum TextureType { BLOCK = 0, PAUSED = 1, WON = 2, LOST = 3 };

	uint32_t mipLevels;
	VkImage textureImages[4];
	VkDeviceMemory textureImageMemorys[4];
	VkImageView textureImageViews[4];
	VkSampler textureSamplers[2];

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool[2];
	std::vector<VkDescriptorSet> descriptorSets_block;
	std::vector<VkDescriptorSet> descriptorSets_gameState;
	VkDescriptorSetLayout descriptorSetLayout_block;
	VkDescriptorSetLayout descriptorSetLayout_gameState;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	bool framebufferResized = false;

	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
	void InitVulkan();

	void CleanUpSwapChain();

	void CleanUp();

	void RecreateSwapChain();

	void CreateInstance();

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void SetUpDebugMessenger();

	void CreateSurface();

	void PickPhysicalDevice();

	void CreateLogicalDevice();

	void CreateSwapChain();

	void CreateImageViews();

	void CreateRenderPass();

	void CreateDescriptorSetLayout_ball();
	void CreateDescriptorSetLayout_gameState();

	void CreateGraphicsPipeline_block();
	void CreateGraphicsPipeline_gameState();

	void CreateFramebuffers();

	void CreateCommandPool();

	void CreateColourResources();

	void CreateDepthResources();

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat FindDepthFormat();

	bool HasStencilComponent(VkFormat format);

	void CreateTextureImage(std::string texturePath, int i);

	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	VkSampleCountFlagBits GetMaxUsableSampleCount();

	void CreateTextures();

	void CreateTextureImageView(int i);

	void CreateTextureSampler(int i);

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void CreateVertexBuffer();

	void CreateIndexBuffer();

	void CreateUniformBuffers();

	void CreateDescriptorPool();

	void CreateDescriptorSets_block();
	void CreateDescriptorSets_gameState();
	void UpdateDescriptorSets_gameState(TextureType type);

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkCommandBuffer BeginSingleTimeCommands();

	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void CreateSyncObjects();

	void UpdateUniformBuffer(uint32_t currentImage);

	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	bool IsDeviceSuitable(VkPhysicalDevice device);

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	std::vector<const char*> GetRequiredExtensions();

	bool CheckValidationLayerSupport();

	static std::vector<char> ReadFile(const std::string& filename);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};
