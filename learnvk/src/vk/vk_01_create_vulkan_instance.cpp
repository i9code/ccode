#include "vk_00_main.h"

/* Vulkan API使用vkInstance对象来存储所有每个应用的状态。应用程序必须在执行任何其他Vulkan操作之前创建一个Vulkan实例，基本的Vulkan架构看起来是这样的：
   Application ->  Loader -> Layer1 ->  ... ->LayerN -> Driver
   请注意，图中有一些层（Layer)，这些层也被加载器加载。层通常用于验证，通常是由驱动执行的错误检查。
   在Vulkan中，驱动程序比OpenGL等其他API要轻量得多，部分原因是它将功能验证委托给验证层。
   层是可选的，每次应用程序创建一个实例时都可以选择性地装载。
*/
void HelloTriangleApplication::createInstance() {
	
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	// 现在我们创建一个instance，并且为该数据结构赋予自定义应用程序的信息。
	// 这些数据从技术角度是可选择的，但是它可以为驱动程序提供一些有用的信息来优化程序特殊的使用情景，
	// 比如驱动程序使用一些图形引擎的特殊行为。这个数据结构称为VkApplicationInfo
	
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;  // Vulkan中的许多数据结构要求在sType成员中明确的指定类型
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	
	// 它需要告知Vulkan驱动程序我们需要使用哪些全局的 extensions 和 validation layers。这里的全局意味着它适用于整个程序，而不是特定的设备，
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	/*
		Vulakn对于平台特性是零API支持的(至少暂时这样)，这意味着需要一个扩展才能与不同平台的窗体系统进行交互。
		GLFW的扩展总是需要的，而debug report扩展是根据编译条件添加。
		与此同时我们使用VK_EXT_DEBUG_REPORT_EXTENSION_NAME宏定义，
		它等价字面值 “VK_EXT_debug_report”，使用宏定义避免了硬编码。
		我们不需要去验证扩展是否存在，因为它会被有效的validation layers引擎的验证。
		*/
	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	// 结构体的最后两个成员确定需要开启的全局的validation layers
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	// 修改VkInstanceCreateInfo结构体，填充当前上下文已经开启的validation layers名称集合
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	// 已经指定了Vulkan创建一个实例需要的一切信息，调用vkCreateInstance创建属于我们的第一个instance:
	/*
	如你所见，Vulkan中创建、实例化相关的函数参数一般遵循如下原则定义:
		使用有关creation info 的结构体指针
		使用自定义分配器回调的指针
		使用保存新对象句柄的指针
		如果一切顺利，此刻instance的句柄应该存储在VkInstance类成员中了。
		几乎所有的Vulkan函数都返回一个值为VK_SUCCESS或错误代码的VkResult类型的值。
		要检查instance是否已经成功创建，我们不需要保存结果，仅仅使用 VK_SUCCESS 值来检测即可：
	*/
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}