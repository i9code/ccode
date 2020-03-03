#include "vk_00_main.h"

// 在将代码传递给渲染管线之前，我们必须将其封装到VkShaderModule对象中。让我们创建一个辅助函数createShaderModule实现该逻辑。
// 该函数需要字节码的缓冲区作为参数，并通过缓冲区创建VkShaderModule。
VkShaderModule HelloTriangleApplication::createShaderModule(const std::vector<char>& code) {
	/*
	创建shader module是比较简单的，我们仅仅需要指定二进制码缓冲区的指针和它的具体长度。
	这些信息被填充在VkShaderModuleCreateInfo结构体中。需要留意的是字节码的大小是以字节指定的，
	但是字节码指针是一个uint32_t类型的指针，而不是一个char指针。所以我们使用reinterpret_cast进行转换指针。
	如下所示，当需要转换时，还需要确保数据满足uint32_t的对齐要求。
	幸运的是，数据存储在std::vector中，默认分配器已经确保数据满足最差情况下的对齐要求。
	*/
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	// 调用vkCreateShaderMoudle创建VkShaderModule:
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}