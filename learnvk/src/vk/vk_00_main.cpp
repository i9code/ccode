#include "vk_00_main.h"
#include <iostream>

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void HelloTriangleApplication::initVulkan() {
	createInstance();

	setupDebugMessenger();
	
	createSurface();

	// 通过VkInstance初始化Vulkan后，
	// 我们需要在系统中查找并选择一个支持我们所需功能的显卡。
	// 实际上，我们可以选择任意数量的显卡并同时使用他们
	// 我们简单的设定选择规则，即将查找到的第一个图形卡作为我们适合的物理设备。
	pickPhysicalDevice();

	createLogicalDevice();

	createSwapChain();
}
	

void HelloTriangleApplication::cleanup() {

	vkDestroySwapchainKHR(device, swapChain, nullptr);

	vkDestroyDevice(device, nullptr);

	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	
	vkDestroySurfaceKHR(instance, surface, nullptr);

	
	glfwDestroyWindow(window);


	vkDestroyInstance(instance, nullptr);


	glfwTerminate();
}


void HelloTriangleApplication::run() {
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}


void HelloTriangleApplication::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}