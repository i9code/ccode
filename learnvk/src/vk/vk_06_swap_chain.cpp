#define GLFW_EXPOSE_NATIVE_WIN32
#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN

#include "vk_00_main.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"


#include "cstdlib"
#include "vector"
#include "algorithm"

void HelloTriangleApplication::createSwapChain() {
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

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

}

//���ڴ����µĺ���querySwapChainSupport���ýṹ��
SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport(VkPhysicalDevice device) {
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
VkSurfaceFormatKHR HelloTriangleApplication::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
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
VkPresentModeKHR HelloTriangleApplication::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
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
VkExtent2D HelloTriangleApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

void HelloTriangleApplication::cleanupSwapChain() {
	//vkDestroyImageView(device, depthImageView, nullptr);
	//vkDestroyImage(device, depthImage, nullptr);
	//vkFreeMemory(device, depthImageMemory, nullptr);

	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	}

	//vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
  

	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(device, swapChain, nullptr);
}