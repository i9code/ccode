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
	// ����ѡ���ͼ���Կ��洢�����ԱVkPhysicalDevice����С�
	// ��VkInstance����ʱ��������󽫻ᱻ��ʽ���٣��������ǲ�����Ҫ��cleanup���������κβ�����
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
	/* Vulkan �߼��豸����У���ѡ��Ҫʹ�õ������豸֮��������Ҫ����һ���߼��豸���ڽ������߼��豸����������instance�����������ƣ�Ҳ��Ҫ����������Ҫʹ�õĹ��ܡ�
	   ��Ϊ�����Ѿ���ѯ����Щ���дؿ��ã���������Ҫ��һ��Ϊ�߼��豸�����������͵�������С�  ����в�ͬ������Ҳ���Ի���ͬһ�������豸��������߼��豸�� */
    // �������һ���µ����Ա���洢�߼��豸�����
	VkDevice device;
	// �������߼��豸�Զ���һͬ�������������ǻ�û��һ�������ǽ��н����ľ�������������һ���µ����Ա���洢ͼ�ζ��о��
	VkQueue presentQueue;
	VkQueue graphicsQueue;

	void createLogicalDevice();

private:
	VkSurfaceKHR surface;
	void createSurface();

private:
	// �������һ�����Ա�����洢VkSwapchainKHR����:
	VkSwapchainKHR swapChain;
	//��������������Ҫ��ȡVkImage��صľ���������ں�����Ⱦ���½������á�������Ա�����洢�þ��:
	std::vector<VkImage> swapChainImages;

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	void createSwapChain();

	//���ڴ����µĺ���querySwapChainSupport���ýṹ��
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

