#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN

#include "vk_00_main.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

void HelloTriangleApplication::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

//  void createSurface() {
	  //VkWin32SurfaceCreateInfoKHR createInfo;
	  //createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	  //createInfo.hwnd = glfwGetWin32Window(window);
	  //createInfo.hinstance = GetModuleHandle(nullptr);

	  //auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");

	  //if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
	  //	throw std::runtime_error("failed to create window surface!");
	  //}
//  }
