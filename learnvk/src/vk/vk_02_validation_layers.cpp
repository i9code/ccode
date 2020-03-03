// validation_layers.cpp
/*
Vulkan 验证层，Vulkan API的设计核心是尽量最小化驱动程序的额外开销，所谓额外开销更多的是指向渲染以外的运算。
其中一个具体的表现就是默认条件下，Vulkan API的错误检查的支持非常有限。
即使遍历不正确的值或者将需要的参数传递为空指针，也不会有明确的处理逻辑，并且直接导致崩溃或者未定义的异常行为。
之所以这样，是因为Vulkan要求每一个步骤定义都非常明确，导致很容易造成小错误，
例如使用新的GPU功能，但是忘记了逻辑设备创建时请求它。

但是，这并不意味着这些检查不能添加到具体的API中。Vulkan推出了一个优化的系统，这个系统称之为Validation layers。
Validation layers是可选组件，可以挂载到Vulkan函数中调用，以回调其他的操作。Validation layers的常见操作情景有:

根据规范检查参数数值，最终确认是否存与预期不符的情况
跟踪对象的创建和销毁，以查找是否存在资源的泄漏
跟踪线程的调用链，确认线程执行过程中的安全性
将每次函数调用所使用的参数记录到标准的输出中，进行初步的Vulkan概要分析
*/

/*
在本节中，我们将介绍如何启用Vulkan SDK提供的标准诊断层。就像扩展一样，需要通过指定具体名称来开启validation layers。
SDK通过请求VK_LAYER_LUNARG_standard_validaction层，来隐式的开启有所关于诊断layers，从而避免明确的指定所有的明确的诊断层。
*/
#include "vk_00_main.h"



VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}



// 让我们看一下callback函数的样子，添加一个静态函数debugCallback,并使用PFN_vkDebugReportCallbackEXT 原型进行修饰。
// VKAPI_ATTR和VKAPI_CALL确保了正确的函数签名，从而被Vulkan调用。
/*
函数的第一个参数指定了消息的类型，它可以通过一下任意标志位组合:
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT = 0x00000001,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT = 0x00000010,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT = 0x00000100,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT = 0x00001000,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF

 objType参数描述作为消息主题的对象的类型，比如一个obj是VkPhysicalDevice，那么objType就是VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT。
	这样做被允许是因为Vulkan的内部句柄都被定义为uint64_t。msg参数包含指向消息的指针。最后，有一个userData参数可将自定义的数据进行回调。

 回调返回一个布尔值，表明触发validation layer消息的Vulkan调用是否应被中止。如果返回true，则调用将以VK_ERROR_VALIDATION_FAILED_EXT错误中止。
 这通常用于测试validation layers本身，所以我们总是返回VK_FALSE。

 现在需要告知Vulkan关于定义的回调函数。也许你会比较惊讶，即使是debug 回调也需要一个明确的创建和销毁句柄的管理工作。添加一个类成员存储回调句柄，在instance下。
*/
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}


void HelloTriangleApplication::setupDebugCallback() {
	if (!enableValidationLayers) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}

}

/*
比较遗憾的是单纯开启validation layers是没有任何帮助的，因为到现在没有任何途径将诊断信息回传给应用程序。要接受消息，我们必须设置回调，需要VK_EXT_debug_report扩展。

我们新增一个getRequiredExtensions函数，该函数将基于是否开启validation layers返回需要的扩展列表
*/
std::vector<const char*> HelloTriangleApplication::getRequiredExtensions() {
	std::vector<const char*> extensions;
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

// 将添加一个新的函数checkValidationLayerSupport,检测所有请求的layers是否可用。
bool HelloTriangleApplication::checkValidationLayerSupport() {
	// 首先使用vkEnumerateInstanceLayerProperties函数列出所有可用的层。
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// 接下来检查validationLayers中的所有layer是否存在于availableLayers列表中。我们需要使用strcmp引入。
	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

