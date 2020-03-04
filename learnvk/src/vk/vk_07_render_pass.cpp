#include "vk_00_main.h"

/*
Vulkan ��Ⱦͨ������������ɹ��ߵĴ�������֮ǰ��������Ҫ����Vulkan��Ⱦʱ��ʹ�õ�framebuffer֡���������������Ϣ��
������Ҫָ�����ٸ���ɫ����Ȼ��������ᱻʹ�ã�ָ�����ٸ����������õ�����������Ⱦ��������ص�������δ���
���е���Щ��Ϣ������װ��һ������ render pass �Ķ����С�
*/
void HelloTriangleApplication::createRenderPass() {
	// �����ǵ������У����ǽ�ֻ��һ����ɫ���������������ɽ������е�һ��ͼ������ʾ��
	VkAttachmentDescription colorAttachment = {};
	//format����ɫ�����ĸ�ʽ����Ӧ���뽻������ͼ��ĸ�ʽ��ƥ�䣬ͬʱ���ǲ������κζ��ز����Ĺ��������Բ���������Ϊ1
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	/*
	oadOp��storeOp��������Ⱦǰ����Ⱦ�������ڶ�Ӧ�����Ĳ�����Ϊ������ loadOp ����������ѡ�
	VK_ATTACHMENT_LOAD_OP_LOAD: �����Ѿ������ڵ�ǰ����������
	VK_ATTACHMENT_LOAD_OP_CLEAR: ��ʼ�׶���һ����������������
	VK_ATTACHMENT_LOAD_OP_DONT_CARE: ���ڵ�����δ���壬��������

	�ڻ����µ�һ֡����֮ǰ������Ҫ������ʹ���������������֡������framebufferΪ��ɫ��ͬʱ���� storeOp ��������ѡ�
	VK_ATTACHMENT_STORE_OP_STORE: ��Ⱦ�����ݻ�洢���ڴ棬����֮����ж�ȡ����
	VK_ATTACHMENT_STORE_OP_DONT_CARE: ֡����������������Ⱦ������Ϻ�����Ϊundefined
	*/
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	// loadOp��storeOpӦ������ɫ��������ݣ�ͬʱstencilLoadOp / stencilStoreOpӦ����ģ�����ݡ����ǵ�Ӧ�ó��򲻻����κ�ģ�滺�����Ĳ�������������loading��storing�޹ؽ�Ҫ��
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	/*
	�����֡��������Vulkan��ͨ����VkImage ��������ĳ�����ظ�ʽ�����������������ڴ��еĲ��ֿ��Ի���ԤҪ��imageͼ����еĲ��������ڴ沼�ֵı仯��
	һЩ���õĲ���:

	VK_IMAGE_LAYOUT_COLOR_ATTACHMET_OPTIMAL: ͼ����Ϊ��ɫ����
	VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: ͼ���ڽ������б�����
	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: ͼ����ΪĿ�꣬�����ڴ�COPY����*/
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
	һ����������Ⱦͨ�������ɶ����ͨ����ɡ���ͨ������Ⱦ������һ�����С�
	��ͨ���������������Ⱦ������������֮ǰ��Ⱦͨ�������֡�����������ݡ�����˵����Ч��������ͨ��ÿһ��������֮ǰ�Ĳ�����
	�������Щ��Ⱦ�������鵽һ����Ⱦͨ���У�ͨ��Vulkan��ͨ���е���Ⱦ�������������򣬿��Խ�ʡ�ڴ�Ӷ���ø��õ����ܡ�
	��������Ҫ���Ƶ������Σ�����ֻ��Ҫһ����ͨ����
	*/
	//ÿ����ͨ������һ�����߶��֮ǰʹ�ýṹ�������ĸ�������Щ���ñ������VkAttachmentReference�ṹ��:
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;//attachment��������ͨ�����������������е����������С����ǵļ�������һ��VkAttachmentDesription��ɵģ�������������Ϊ0
	
	//layoutΪ����ָ����ͨ���ڳ�������ʱ���layout������ͨ����ʼ��ʱ��Vulkan���Զ�ת�丽�������layout��
	//��Ϊ����������������ɫ�����������ã�layout����ΪVK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL���������õ����ܡ�
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//��ͨ��ʹ��VkSubpassDescription�ṹ������
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	
/*	�����������е�����ֱ�Ӵ�Ƭ����ɫ�����ã���layout(location = 0) out vec4 outColor ָ��!
	���Ա���ͨ�����õĸ�����������:
	pInputAttachments: ��������ɫ���ж�ȡ
	pResolveAttachments : ����������ɫ�����Ķ��ز���
	pDepthStencilAttachment : ����������Ⱥ�ģ������
	pPreserveAttachments : ����������ͨ��ʹ�ã��������ݱ�����*/
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

	//��Ⱦͨ�����󴴽�ͨ�����VkRenderPassCreateInfo�ṹ�壬�������ظ�������ͨ������ɡ�VkAttachmentReference�������ø������顣
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
