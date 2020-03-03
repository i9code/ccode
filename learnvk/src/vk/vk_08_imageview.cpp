#include "vk_00_main.h"


void HelloTriangleApplication::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

VkImageView HelloTriangleApplication::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
	// 创建图像视图的参数被定义在VkImageViewCreateInfo结构体中。前几个参数的填充非常简单、直接。
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;

	// 其中viewType和format字段用于描述图像数据该被如何解释。viewType参数允许将图像定义为1D textures, 2D textures, 3D textures 和cube maps。
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;


	/*
		components字段允许调整颜色通道的最终的映射逻辑。比如，我们可以将所有颜色通道映射为红色通道，以实现单色纹理。
		我们也可以将通道映射具体的常量数值0和1。在章节中我们使用默认的映射策略。
	*/
	viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	/*
		subresourceRangle字段用于描述图像的使用目标是什么，以及可以被访问的有效区域。
		我们的图像将会作为color targets，没有任何mipmapping levels 或是多层 multiple layers。
	*/
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

