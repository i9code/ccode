#include "vk_00_main.h"

// �ڽ����봫�ݸ���Ⱦ����֮ǰ�����Ǳ��뽫���װ��VkShaderModule�����С������Ǵ���һ����������createShaderModuleʵ�ָ��߼���
// �ú�����Ҫ�ֽ���Ļ�������Ϊ��������ͨ������������VkShaderModule��
VkShaderModule HelloTriangleApplication::createShaderModule(const std::vector<char>& code) {
	/*
	����shader module�ǱȽϼ򵥵ģ����ǽ�����Ҫָ���������뻺������ָ������ľ��峤�ȡ�
	��Щ��Ϣ�������VkShaderModuleCreateInfo�ṹ���С���Ҫ��������ֽ���Ĵ�С�����ֽ�ָ���ģ�
	�����ֽ���ָ����һ��uint32_t���͵�ָ�룬������һ��charָ�롣��������ʹ��reinterpret_cast����ת��ָ�롣
	������ʾ������Ҫת��ʱ������Ҫȷ����������uint32_t�Ķ���Ҫ��
	���˵��ǣ����ݴ洢��std::vector�У�Ĭ�Ϸ������Ѿ�ȷ�����������������µĶ���Ҫ��
	*/
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	// ����vkCreateShaderMoudle����VkShaderModule:
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}