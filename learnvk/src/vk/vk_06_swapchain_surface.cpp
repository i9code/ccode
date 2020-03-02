// logical_device.cpp
#define GLFW_EXPOSE_NATIVE_WIN32
#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "GLFW/glfw3native.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <set>
#include <algorithm>

const int WIDTH = 800;
const int HEIGHT = 600;

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

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    /*
    Vulkan �߼��豸����У���ѡ��Ҫʹ�õ������豸֮��������Ҫ����һ���߼��豸���ڽ�����
    �߼��豸����������instance�����������ƣ�Ҳ��Ҫ����������Ҫʹ�õĹ��ܡ�
    ��Ϊ�����Ѿ���ѯ����Щ���дؿ��ã���������Ҫ��һ��Ϊ�߼��豸�����������͵�������С�
    ����в�ͬ������Ҳ���Ի���ͬһ�������豸��������߼��豸��
    */
    // �������һ���µ����Ա���洢�߼��豸�����
    VkDevice device;

    // �������߼��豸�Զ���һͬ�������������ǻ�û��һ�������ǽ��н����ľ�������������һ���µ����Ա���洢ͼ�ζ��о��
    VkQueue graphicsQueue;

    

    // �������һ�����Ա�����洢VkSwapchainKHR����:
    VkSwapchainKHR swapChain;
    //��������������Ҫ��ȡVkImage��صľ���������ں�����Ⱦ���½������á�������Ա�����洢�þ��:
    std::vector<VkImage> swapChainImages;
	
    VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyDevice(device, nullptr);

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void createInstance() {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
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

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

	void createSurface() {
		VkWin32SurfaceCreateInfoKHR createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = glfwGetWin32Window(window);
        createInfo.hinstance = GetModuleHandle(nullptr);

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

    void pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void createLogicalDevice() {
        /*
        �����߼��豸��Ҫ�ڽṹ������ȷ�������Ϣ��
        ���ȵ�һ���ṹ��VkDeviceQueueCreateInfo��
        ����ṹ���������д���ԤҪ����ʹ�õĶ���������
        �������ǽ����ľ߱�ͼ�������Ķ��С�
        */
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
        queueCreateInfo.queueCount = 1;

        // Vulkan����ʹ��0.0��1.0֮��ĸ���������������ȼ���Ӱ���������ִ�еĵ��á���ʹֻ��һ������Ҳ�Ǳ����:
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;


        /*
        ��һ��Ҫ��ȷ����Ϣ�й��豸Ҫʹ�õĹ������ԡ�
        ��Щ����������һ������vkGetPhysicalDeviceFeatures��ѯ֧�ֵĹ��ܣ�����geometry shaders��
        �������ǲ���Ҫ�κ�����Ĺ��ܣ��������ǿ��Լ򵥵Ķ����������������ݱ�����VK_FALSE��
        һ������Ҫ��ʼ��Vulkan����������飬���ǻ�ص�����ṹ�壬��һ�����á�*/
        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // ����Ϊinstance����validation layers���������ǽ�Ϊ�豸����validation layers��������ҪΪ�豸ָ���κ���չ��
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        // ���ڿ���ͨ������vkCreateDevice����������ʵ�����߼��豸��
        // Щ�����ֱ��ǰ����������ʹ����Ϣ�������豸����ѡ�ķ������ص�ָ���Լ����ڴ洢�߼��豸�ľ����
        // ��instance�������ƣ��˵��ÿ����������ò����ڵ���չ����ָ����֧�ֵĹ��ܣ����·��ش���
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }
        /*
           ʹ��vkGetDeviceQueue���������ÿ�����д��ж��еľ�����������߼��豸�����дأ����������ʹ洢��ȡ���б��������ָ�롣
           ��Ϊ����ֻ�Ǵ�������дش���һ�����У�������Ҫʹ������ 0��
           �ڳɹ���ȡ�߼��豸�Ͷ��о�������ǿ���ͨ���Կ���һЩʵ�ʵ�������
        */
        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
    }

    bool isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
   
    }

	bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.graphicsFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

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

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

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

    //���ڴ����µĺ���querySwapChainSupport���ýṹ��
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;
        //���ڿ�ʼ������surface�������ò��֡���Щ���Կ���ͨ���򵥵ĺ������ò�ѯ�������ص�����VkSurfaceCapabilitiesKHR�ṹ���С�
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}
		return details;
	}
    // Ϊ������ѡ����ȷ������
    /*
	���swapChainAdequate�����㹻����ô��Ӧ��֧�ֵ��㹻�ģ����Ǹ��ݲ�ͬ��ģʽ��Ȼ�в�ͬ�����ѡ�����Ǳ�дһ�麯����ͨ����һ�������ò�����ƥ��Ľ��������������������͵�����ȥȷ��:

    Surface��ʽ (color depth)
    Presentation mode (conditions for ��swapping�� image to the screen)
    Swap extent (resolution of images in swap chain)
    �������Ժ��ж�ÿһ�����ö���һ���������ֵ��������һ�����Ǿ�ʹ�ã���������һ�𴴽�һЩ�߼�ȥ�ҵ����õĹ�����ֵ��
    */
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        /*
		ÿ��VkSurfaceFormatKHR�ṹ������һ��format��һ��colorSpace��Ա��
        format��Ա����ָ��ɫ��ͨ�������͡����磬VK_FORMAT_B8G8R8A8_UNORM����������ʹ��B,G,R��alpha�����ͨ������ÿһ��ͨ��Ϊ�޷���8bit������ÿ�������ܼ�32bits��
        colorSpace��Ա����SRGB��ɫ�ռ��Ƿ�ͨ��VK_COLOR_SPACE_SRGB_NONLINEAR_KHR��־֧�֡���Ҫע������ڽ���汾�Ĺ淶�У������־��ΪVK_COLORSPACE_SRGB_NONLINEAR_KHR��

        ����������Ǿ�����ʹ��SRGB(��ɫ����Э��)����Ϊ����õ������׸�֪�ġ���ȷ��ɫ�ʡ�ֱ����SRGB��ɫ�򽻵��ǱȽ�����ս�ģ�
        ��������ʹ�ñ�׼��RGB��Ϊ��ɫ��ʽ����Ҳ��ͨ��ʹ�õ�һ����ʽVK_FORMAT_B8G8R8A8_UNORM��
        
        */
        // ������������surfaceû�������κ�ƫ���Եĸ�ʽ�����ʱ��Vulkan��ͨ��������һ��VkSurfaceFormatKHR�ṹ��ʾ���Ҹýṹ��format��Ա����ΪVK_FORMAT_UNDEFINED��
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}
        //����������ɵ����ø�ʽ����ô���ǿ���ͨ�������б����þ���ƫ���Ե����:
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
        // ����������ַ�ʽ��ʧЧ�ˣ����ʱ�����ǿ���ͨ�������������д�����򣬵��Ǵ��������»�ѡ���һ����ʽ��Ϊ�����ѡ��
        return availableFormats[0];
	}
	
    // ��ʾģʽ
    /*
	presentationģʽ���ڽ������Ƿǳ���Ҫ�ģ���Ϊ������������Ļ����ͼ�����������Vulkan�����ĸ�ģʽ����ʹ��:

	VK_PRESENT_MODE_IMMEDIATE_KHR: Ӧ�ó����ύ��ͼ���������䵽��Ļ���֣�����ģʽ���ܻ����˺��Ч����
	
    VK_PRESENT_MODE_FIFO_KHR: ������������һ�����У�����ʾ������Ҫˢ�µ�ʱ����ʾ�豸�Ӷ��е�ǰ���ȡͼ�񣬲��ҳ�����Ⱦ��ɵ�ͼ�������еĺ��档
    ������������ĳ����ȴ������ֹ�ģ����Ƶ��Ϸ�Ĵ�ֱͬ�������ơ���ʾ�豸��ˢ��ʱ�̱���Ϊ����ֱ�жϡ���
	
    VK_PRESENT_MODE_FIFO_RELAXED_KHR: ��ģʽ����һ��ģʽ���в�ͬ�ĵط�Ϊ�����Ӧ�ó�������ӳ٣�
    ���������һ����ֱͬ���ź�ʱ���п��ˣ�������ȴ���һ����ֱͬ���źţ����ǽ�ͼ��ֱ�Ӵ��͡����������ܵ��¿ɼ���˺��Ч����


	VK_PRESENT_MODE_MAILBOX_KHR: ���ǵڶ���ģʽ�ı��֡�����������������ʱ��ѡ���µ��滻�ɵ�ͼ�񣬴Ӷ��������Ӧ�ó�������Ρ�
    ����ģʽͨ������ʵ�����ػ����������׼�Ĵ�ֱͬ��˫������ȣ���������Ч�����ӳٴ�����˺��Ч����
    */

    // �߼��Ͽ�����VR_PRESENT_MODE_FIFO_KHRģʽ��֤�����ԣ����������ٴ�����һ������������ѵ�ģʽ:
    //�ź����ǣ�һЩ��������Ŀǰ����֧��VK_PRESENT_MODE_FIFO_KHR,����֮�����VK_PRESENT_MODE_MAILBOX_KHRҲ�����ã����Ǹ�����ʹ��VK_PRESENT_MODE_IMMEDIATE_KHR:
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				bestMode = availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}
    /*
    ������Χ��ָ������ͼ��ķֱ��ʣ����������ǵ������ǻ��ƴ���ķֱ��ʡ�
    �ֱ��ʵķ�Χ��������VkSurfaceCapabilitiesKHR�ṹ���С�Vulkan��������ͨ������currentExtent��Ա��width��height��ƥ�䴰��ķֱ��ʡ�
    Ȼ����һЩ�������������ͬ�����ã���ζ�Ž�currentExtent��width��height����Ϊ�������ֵ��ʾ:uint32_t�����ֵ��
    ����������£����ǲο�����minImageExtent��maxImageExtentѡ����ƥ��ķֱ��ʡ�
    */
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { WIDTH, HEIGHT };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	void createSwapChain() {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        //��һ���ǽ������е�ͼ���������������Ϊ���еĳ��ȡ���ָ������ʱͼ�����С���������ǽ����Դ���1��ͼ����������ʵ�����ػ��塣
        // ����maxImageCount��ֵΪ0��������ڴ�֮��û������
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

        //��Vulkan��������Ĵ�������һ��������������Ҳ��Ҫ�������Ľṹ��:
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
        // ��ָ���������󶨵������surface֮����Ҫָ��������ͼ���йص���ϸ��Ϣ:
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;  //imageArrayLayersָ��ÿ��ͼ����ɵĲ������������ǿ���3DӦ�ó��򣬷���ʼ��Ϊ1
        // imageUsageλ�ֶ�ָ���ڽ������ж�ͼ����еľ���������ڱ�С���У����ǽ�ֱ�Ӷ����ǽ�����Ⱦ������ζ��������Ϊ��ɫ������
        // Ҳ�������Ƚ�ͼ����ȾΪ������ͼ�񣬽��к��������
        // ����������¿���ʹ����VK_IMAGE_USAGE_TRANSFER_DST_BIT������ֵ����ʹ���ڴ��������Ⱦ��ͼ���䵽������ͼ����С�
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
        /*
		��������������Ҫָ����δ���������дصĽ�����ͼ�����graphics���д���presentation���дز�ͬ��������������Ρ�
        ���ǽ���graphics�����л��ƽ�������ͼ��Ȼ������һ��presentation�������ύ���ǡ�����д���ͼ�������ַ���:

        VK_SHARING_MODE_EXCLUSIVE: ͬһʱ��ͼ��ֻ�ܱ�һ�����д�ռ�ã�����������д���Ҫ������Ȩ��Ҫ��ȷָ�������ַ�ʽ�ṩ����õ����ܡ�
        VK_SHARING_MODE_CONCURRENT: ͼ����Ա�������дط��ʣ�����Ҫ��ȷ����Ȩ������ϵ��
        �ڱ�С���У�������дز�ͬ������ʹ��concurrentģʽ�����⴦��ͼ������Ȩ������ϵ�����ݣ���Ϊ��Щ���漰���ٸ������������½����ۡ�
        Concurrentģʽ��ҪԤ��ָ�����д�����Ȩ������ϵ��ͨ��queueFamilyIndexCount��pQueueFamilyIndices�������й���
        ���graphics���дغ�presentation���д���ͬ��������Ҫʹ��exclusiveģʽ����Ϊconcurrentģʽ��Ҫ����������ͬ�Ķ��дء�
        */
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.graphicsFamily };

		if (indices.graphicsFamily != indices.graphicsFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}
        // ���������֧��(supportedTransforms in capabilities),���ǿ���Ϊ������ͼ��ָ��ĳЩת���߼���
        // ����90��˳ʱ����ת����ˮƽ��ת���������Ҫ�κ�transoform���������Լ򵥵�����ΪcurrentTransoform��
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        //���Alpha�ֶ�ָ��alphaͨ���Ƿ�Ӧ�����������Ĵ���ϵͳ���л�ϲ�����������Ըù��ܣ��򵥵���VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR��
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        /*
			presentModeָ���Լ������clipped��Ա����ΪVK_TRUE����ζ�����ǲ����ı��ڱε��������ݣ�
			�������������Ĵ�������ǰ��ʱ������Ⱦ�Ĳ������ݴ����ڿ�������֮�⣬���������Ҫ��ȡ��Щ���ػ����ݽ��д���������Կ����ü����������ܡ�
        */
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
        //���һ���ֶ�oldSwapChain��Vulkan����ʱ��������������ĳЩ�����±��滻�����細�ڵ�����С���߽�������Ҫ���·�������ͼ����С�
        //����������£�������ʵ������Ҫ���·��䴴�������ұ����ڴ��ֶ���ָ���Ծɵ����ã����Ի�����Դ��
        //����һ���Ƚϸ��ӵĻ��⣬���ǻ��ں�����½�����ϸ���ܡ����ڼ�������ֻ�ᴴ��һ����������
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        // �������߼��豸����������������Ϣ����ѡ��ķ�������һ���洢������ľ��ָ�롣��Ҳ��Ҫ���豸������ǰ���������ٲ�����ͨ������vkDestroySwapchainKHR��
       
		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

        // ����ͨ������vkGetSwapchainImagesKHR��ȡ��������ͼ����������������������ú��ʵ�������С�����ȡ���ľ�����ϡ�
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	}
};

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