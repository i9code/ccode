// logical_device.cpp
#include "vk_00_main.h"
#include "set"




void HelloTriangleApplication::createLogicalDevice() {
    /*
    创建逻辑设备需要在结构体中明确具体的信息，首先第一个结构体VkDeviceQueueCreateInfo。
    这个结构体描述队列簇中预要申请使用的队列数量。现在我们仅关心具备图形能力的队列。
    */
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };


	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority; // // Vulkan允许使用0.0到1.0之间的浮点数分配队列优先级来影响命令缓冲区执行的调用。即使只有一个队列也是必须的:
		queueCreateInfos.push_back(queueCreateInfo);
	}

    /*
    下一个要明确的信息有关设备要使用的功能特性。
    这些是我们在上一节中用vkGetPhysicalDeviceFeatures查询支持的功能，比如geometry shaders。
    现在我们不需要任何特殊的功能，所以我们可以简单的定义它并将所有内容保留到VK_FALSE。
    一旦我们要开始用Vulkan做更多的事情，我们会回到这个结构体，进一步设置。*/
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
 

    createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // 允许为instance开启validation layers，现在我们将为设备开启validation layers，而不需要为设备指定任何扩展。
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    // 现在可以通过调用vkCreateDevice函数来创建实例化逻辑设备。
    // 些参数分别是包含具体队列使用信息的物理设备，可选的分配器回调指针以及用于存储逻辑设备的句柄。
    // 与instance创建类似，此调用可能由于启用不存在的扩展或者指定不支持的功能，导致返回错误。
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    /*
       使用vkGetDeviceQueue函数来检测每个队列簇中队列的句柄。参数是逻辑设备，队列簇，队列索引和存储获取队列变量句柄的指针。
       因为我们只是从这个队列簇创建一个队列，所以需要使用索引 0。
       在成功获取逻辑设备和队列句柄后，我们可以通过显卡做一些实际的事情了
    */
	vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
}
