#include "vk_00_main.h"

/*
Vulkan 渲染通道，在我们完成管线的创建工作之前，我们需要告诉Vulkan渲染时候使用的framebuffer帧缓冲区附件相关信息。
我们需要指定多少个颜色和深度缓冲区将会被使用，指定多少个采样器被用到及在整个渲染操作中相关的内容如何处理。
所有的这些信息都被封装在一个叫做 render pass 的对象中。
*/
void HelloTriangleApplication::createRenderPass() {
	// 在我们的例子中，我们将只有一个颜色缓冲区附件，它由交换链中的一个图像所表示。
	VkAttachmentDescription colorAttachment = {};
	//format是颜色附件的格式，它应该与交换链中图像的格式相匹配，同时我们不会做任何多重采样的工作，所以采样器设置为1
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	/*
	oadOp和storeOp决定了渲染前和渲染后数据在对应附件的操作行为。对于 loadOp 我们有如下选项：
	VK_ATTACHMENT_LOAD_OP_LOAD: 保存已经存在于当前附件的内容
	VK_ATTACHMENT_LOAD_OP_CLEAR: 起始阶段以一个常量清理附件内容
	VK_ATTACHMENT_LOAD_OP_DONT_CARE: 存在的内容未定义，忽略它们

	在绘制新的一帧内容之前，我们要做的是使用清理操作来清理帧缓冲区framebuffer为黑色。同时对于 storeOp 仅有两个选项：
	VK_ATTACHMENT_STORE_OP_STORE: 渲染的内容会存储在内存，并在之后进行读取操作
	VK_ATTACHMENT_STORE_OP_DONT_CARE: 帧缓冲区的内容在渲染操作完毕后设置为undefined
	*/
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	// loadOp和storeOp应用在颜色和深度数据，同时stencilLoadOp / stencilStoreOp应用在模版数据。我们的应用程序不会做任何模版缓冲区的操作，所以它的loading和storing无关紧要。
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	/*
	纹理和帧缓冲区在Vulkan中通常用VkImage 对象配以某种像素格式来代表。但是像素在内存中的布局可以基于预要对image图像进行的操作发生内存布局的变化。
	一些常用的布局:

	VK_IMAGE_LAYOUT_COLOR_ATTACHMET_OPTIMAL: 图像作为颜色附件
	VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: 图像在交换链中被呈现
	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: 图像作为目标，用于内存COPY操作*/
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


	/*
	一个单独的渲染通道可以由多个子通道组成。子通道是渲染操作的一个序列。
	子通道作用与后续的渲染操作，并依赖之前渲染通道输出到帧缓冲区的内容。比如说后处理效果的序列通常每一步都依赖之前的操作。
	如果将这些渲染操作分组到一个渲染通道中，通过Vulkan将通道中的渲染操作进行重排序，可以节省内存从而获得更好的性能。
	对于我们要绘制的三角形，我们只需要一个子通道。
	*/
	//每个子通道引用一个或者多个之前使用结构体描述的附件。这些引用本身就是VkAttachmentReference结构体:
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;//attachment附件参数通过附件描述符集合中的索引来持有。我们的集合是由一个VkAttachmentDesription组成的，所以它的索引为0
	
	//layout为附件指定子通道在持有引用时候的layout。当子通道开始的时候Vulkan会自动转变附件到这个layout。
	//因为我们期望附件起到颜色缓冲区的作用，layout设置为VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL会给我们最好的性能。
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//子通道使用VkSubpassDescription结构体描述
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	
/*	附件在数组中的索引直接从片段着色器引用，其layout(location = 0) out vec4 outColor 指令!
	可以被子通道引用的附件类型如下:
	pInputAttachments: 附件从着色器中读取
	pResolveAttachments : 附件用于颜色附件的多重采样
	pDepthStencilAttachment : 附件用于深度和模版数据
	pPreserveAttachments : 附件不被子通道使用，但是数据被保存*/
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

	//渲染通道对象创建通过填充VkRenderPassCreateInfo结构体，并配合相关附件和子通道来完成。VkAttachmentReference对象引用附件数组。
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

VkFormat HelloTriangleApplication::findDepthFormat() {
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

VkFormat HelloTriangleApplication::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}
