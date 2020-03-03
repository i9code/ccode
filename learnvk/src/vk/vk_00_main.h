#pragma once

// 01_instance_creation.cpp
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstdlib>
#include "iostream"
#include "vector"

const int WIDTH = 800;
const int HEIGHT = 600;

//�����ڳ���������������ñ�����ָ��Ҫ���õ�layers�Լ��Ƿ������ǡ�
//����ѡ����ڳ����Ƿ��ڵ���ģʽ�½��б��롣NDEBUG��C++��׼�궨�壬���������ԡ�
const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

// ������������ĳ�����ԵĶ��д�����������ṹ��
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		// return graphicsFamily >= 0 && presentFamily >= 0;
		return graphicsFamily >= 0;
	}
};

/*
���������Ϊ�˲��Խ���������Ч����ԶԶ�����ģ���Ϊ�������ܺܺõ��봰��surface���ݡ�����������ͬ��Ҳ��Ҫ�ܶ����ã�����������Ҫ�˽�һЩ�й����õ�ϸ�ڡ�

��������������������Ҫ����:
	������surface��������(min/max number of images in swap chain, min/max width and height of images)
	Surface��ʽ(pixel format, color space)
	��Ч��presentationģʽ
	��findQueueFamilies���ƣ�����ʹ�ýṹ��һ���ԵĴ�����ϸ����Ϣ���������Է�װ�����½ṹ���У�
*/
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


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
	VkDebugUtilsMessengerEXT debugMessenger;

	// debug and  physical device selection
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
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
	
	
};

