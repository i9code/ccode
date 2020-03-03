#include "vk_00_main.h"

/* Vulkan APIʹ��vkInstance�������洢����ÿ��Ӧ�õ�״̬��Ӧ�ó��������ִ���κ�����Vulkan����֮ǰ����һ��Vulkanʵ����������Vulkan�ܹ��������������ģ�
   Application ->  Loader -> Layer1 ->  ... ->LayerN -> Driver
   ��ע�⣬ͼ����һЩ�㣨Layer)����Щ��Ҳ�����������ء���ͨ��������֤��ͨ����������ִ�еĴ����顣
   ��Vulkan�У����������OpenGL������APIҪ�����ö࣬����ԭ��������������֤ί�и���֤�㡣
   ���ǿ�ѡ�ģ�ÿ��Ӧ�ó��򴴽�һ��ʵ��ʱ������ѡ���Ե�װ�ء�
*/
void HelloTriangleApplication::createInstance() {
	
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	// �������Ǵ���һ��instance������Ϊ�����ݽṹ�����Զ���Ӧ�ó������Ϣ��
	// ��Щ���ݴӼ����Ƕ��ǿ�ѡ��ģ�����������Ϊ���������ṩһЩ���õ���Ϣ���Ż����������ʹ���龰��
	// ������������ʹ��һЩͼ�������������Ϊ��������ݽṹ��ΪVkApplicationInfo
	
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;  // Vulkan�е�������ݽṹҪ����sType��Ա����ȷ��ָ������
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	
	// ����Ҫ��֪Vulkan��������������Ҫʹ����Щȫ�ֵ� extensions �� validation layers�������ȫ����ζ�����������������򣬶������ض����豸��
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	/*
		Vulakn����ƽ̨��������API֧�ֵ�(������ʱ����)������ζ����Ҫһ����չ�����벻ͬƽ̨�Ĵ���ϵͳ���н�����
		GLFW����չ������Ҫ�ģ���debug report��չ�Ǹ��ݱ���������ӡ�
		���ͬʱ����ʹ��VK_EXT_DEBUG_REPORT_EXTENSION_NAME�궨�壬
		���ȼ�����ֵ ��VK_EXT_debug_report����ʹ�ú궨�������Ӳ���롣
		���ǲ���Ҫȥ��֤��չ�Ƿ���ڣ���Ϊ���ᱻ��Ч��validation layers�������֤��
		*/
	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	// �ṹ������������Աȷ����Ҫ������ȫ�ֵ�validation layers
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	// �޸�VkInstanceCreateInfo�ṹ�壬��䵱ǰ�������Ѿ�������validation layers���Ƽ���
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

	// �Ѿ�ָ����Vulkan����һ��ʵ����Ҫ��һ����Ϣ������vkCreateInstance�����������ǵĵ�һ��instance:
	/*
	����������Vulkan�д�����ʵ������صĺ�������һ����ѭ����ԭ����:
		ʹ���й�creation info �Ľṹ��ָ��
		ʹ���Զ���������ص���ָ��
		ʹ�ñ����¶�������ָ��
		���һ��˳�����˿�instance�ľ��Ӧ�ô洢��VkInstance���Ա���ˡ�
		�������е�Vulkan����������һ��ֵΪVK_SUCCESS���������VkResult���͵�ֵ��
		Ҫ���instance�Ƿ��Ѿ��ɹ����������ǲ���Ҫ������������ʹ�� VK_SUCCESS ֵ����⼴�ɣ�
	*/
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}