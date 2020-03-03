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

	// ͨ��VkInstance��ʼ��Vulkan��
	// ������Ҫ��ϵͳ�в��Ҳ�ѡ��һ��֧���������蹦�ܵ��Կ���
	// ʵ���ϣ����ǿ���ѡ�������������Կ���ͬʱʹ������
	// ���Ǽ򵥵��趨ѡ����򣬼������ҵ��ĵ�һ��ͼ�ο���Ϊ�����ʺϵ������豸��
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