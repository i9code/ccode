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
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>

const int WIDTH = 800;
const int HEIGHT = 600;


//首先在程序中添加两个配置变量来指定要启用的layers以及是否开启它们。
//我们选择基于程序是否在调试模式下进行编译。NDEBUG是C++标准宏定义，代表“不调试”
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
      /*
      该结构体应该传递给CreateDebugUtilsMessengerEXT函数创建对象。
      不幸的是，因为这个功能是一个扩展功能，它不会被自动加载。
      所以必须使用vkGetInstanceProcAddr查找函数地址。我们将在后台创建代理函数。在HelloTriangleApplication类定义之上添加它。
      */
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

    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        setupDebugMessenger();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
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

        /*
            GLFW的扩展总是需要的，而debug report扩展是根据编译条件添加。
            与此同时我们使用VK_EXT_DEBUG_REPORT_EXTENSION_NAME宏定义，
            它等价字面值 “VK_EXT_debug_report”，使用宏定义避免了硬编码。
            我们不需要去验证扩展是否存在，因为它会被有效的validation layers引擎的验证。
        */
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

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


    /*
    比较遗憾的是单纯开启validation layers是没有任何帮助的，因为到现在没有任何途径将诊断信息回传给应用程序。要接受消息，我们必须设置回调，需要VK_EXT_debug_report扩展。

    我们新增一个getRequiredExtensions函数，该函数将基于是否开启validation layers返回需要的扩展列表
    */
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

    // 将添加一个新的函数checkValidationLayerSupport,检测所有请求的layers是否可用。
    bool checkValidationLayerSupport() {
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
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
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