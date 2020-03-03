// validation_layers.cpp
/*
Vulkan ��֤�㣬Vulkan API����ƺ����Ǿ�����С����������Ķ��⿪������ν���⿪���������ָ����Ⱦ��������㡣
����һ������ı��־���Ĭ�������£�Vulkan API�Ĵ������֧�ַǳ����ޡ�
��ʹ��������ȷ��ֵ���߽���Ҫ�Ĳ�������Ϊ��ָ�룬Ҳ��������ȷ�Ĵ����߼�������ֱ�ӵ��±�������δ������쳣��Ϊ��
֮��������������ΪVulkanҪ��ÿһ�����趨�嶼�ǳ���ȷ�����º��������С����
����ʹ���µ�GPU���ܣ������������߼��豸����ʱ��������

���ǣ��Ⲣ����ζ����Щ��鲻�����ӵ������API�С�Vulkan�Ƴ���һ���Ż���ϵͳ�����ϵͳ��֮ΪValidation layers��
Validation layers�ǿ�ѡ��������Թ��ص�Vulkan�����е��ã��Իص������Ĳ�����Validation layers�ĳ��������龰��:

���ݹ淶��������ֵ������ȷ���Ƿ����Ԥ�ڲ��������
���ٶ���Ĵ��������٣��Բ����Ƿ������Դ��й©
�����̵߳ĵ�������ȷ���߳�ִ�й����еİ�ȫ��
��ÿ�κ���������ʹ�õĲ�����¼����׼������У����г�����Vulkan��Ҫ����
*/

/*
�ڱ����У����ǽ������������Vulkan SDK�ṩ�ı�׼��ϲ㡣������չһ������Ҫͨ��ָ����������������validation layers��
SDKͨ������VK_LAYER_LUNARG_standard_validaction�㣬����ʽ�Ŀ��������������layers���Ӷ�������ȷ��ָ�����е���ȷ����ϲ㡣
*/
#include "vk_00_main.h"




VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}


// �����ǿ�һ��callback���������ӣ�����һ����̬����debugCallback,��ʹ��PFN_vkDebugReportCallbackEXT ԭ�ͽ������Ρ�
// VKAPI_ATTR��VKAPI_CALLȷ������ȷ�ĺ���ǩ�����Ӷ���Vulkan���á�
/*
�����ĵ�һ������ָ������Ϣ�����ͣ�������ͨ��һ�������־λ���:
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT = 0x00000001,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT = 0x00000010,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT = 0x00000100,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT = 0x00001000,
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF

 objType����������Ϊ��Ϣ����Ķ�������ͣ�����һ��obj��VkPhysicalDevice����ôobjType����VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT��
	����������������ΪVulkan���ڲ������������Ϊuint64_t��msg��������ָ����Ϣ��ָ�롣�����һ��userData�����ɽ��Զ�������ݽ��лص���

 �ص�����һ������ֵ����������validation layer��Ϣ��Vulkan�����Ƿ�Ӧ����ֹ���������true������ý���VK_ERROR_VALIDATION_FAILED_EXT������ֹ��
 ��ͨ�����ڲ���validation layers�����������������Ƿ���VK_FALSE��

 ������Ҫ��֪Vulkan���ڶ���Ļص�������Ҳ�����ȽϾ��ȣ���ʹ��debug �ص�Ҳ��Ҫһ����ȷ�Ĵ��������پ���Ĺ�������������һ�����Ա�洢�ص��������instance�¡�
*/
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}



void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

void HelloTriangleApplication::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

/*
�Ƚ��ź����ǵ�������validation layers��û���κΰ����ģ���Ϊ������û���κ�;���������Ϣ�ش���Ӧ�ó���Ҫ������Ϣ�����Ǳ������ûص�����ҪVK_EXT_debug_report��չ��

��������һ��getRequiredExtensions�������ú����������Ƿ���validation layers������Ҫ����չ�б�
*/
std::vector<const char*> HelloTriangleApplication::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

// ������һ���µĺ���checkValidationLayerSupport,������������layers�Ƿ���á�
bool HelloTriangleApplication::checkValidationLayerSupport() {
	// ����ʹ��vkEnumerateInstanceLayerProperties�����г����п��õĲ㡣
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// ���������validationLayers�е�����layer�Ƿ������availableLayers�б��С�������Ҫʹ��strcmp���롣
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
