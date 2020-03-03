#pragma once

// 01_instance_creation.cpp
#define GLFW_INCLUDE_VULKAN

#include "vk_00_define.h"
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <cstdlib>
#include "iostream"
#include "vector"

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);


class HelloTriangleApplication {
public:
	void run();

private:
	GLFWwindow* window;

	VkInstance instance;

	

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

private:
	// 我们选择的图形显卡存储在类成员VkPhysicalDevice句柄中。
	// 当VkInstance销毁时，这个对象将会被隐式销毁，所以我们并不需要在cleanup函数中做任何操作。
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	void createInstance();
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

private: 
	VkDebugReportCallbackEXT callback;

	// debug and  physical device selection
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugCallback();
	void pickPhysicalDevice();
	std::vector<const char*> getRequiredExtensions();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkValidationLayerSupport();

private:
	/* Vulkan 逻辑设备与队列，在选择要使用的物理设备之后，我们需要设置一个逻辑设备用于交互。逻辑设备创建过程与instance创建过程类似，也需要描述我们需要使用的功能。
	   因为我们已经查询过哪些队列簇可用，在这里需要进一步为逻辑设备创建具体类型的命令队列。  如果有不同的需求，也可以基于同一个物理设备创建多个逻辑设备。 */
    // 首先添加一个新的类成员来存储逻辑设备句柄。
	VkDevice device;
	// 队列与逻辑设备自动的一同创建，但是我们还没有一个与它们进行交互的句柄。在这里添加一个新的类成员来存储图形队列句柄
	VkQueue presentQueue;
	VkQueue graphicsQueue;

	void createLogicalDevice();

private:
	VkSurfaceKHR surface;
	void createSurface();

private:
	// 现在添加一个类成员变量存储VkSwapchainKHR对象:
	VkSwapchainKHR swapChain;
	//交换链创建后，需要获取VkImage相关的句柄。它会在后续渲染的章节中引用。添加类成员变量存储该句柄:
	std::vector<VkImage> swapChainImages;

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	void createSwapChain();

	//现在创建新的函数querySwapChainSupport填充该结构体
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	

private:
	// vkimage
	std::vector<VkImageView> swapChainImageViews;
	void createImageViews();
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	
};

