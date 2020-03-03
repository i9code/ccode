// logical_device.cpp
#include "vk_00_main.h"


void HelloTriangleApplication::createLogicalDevice() {
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

    createInfo.enabledExtensionCount = 0;

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