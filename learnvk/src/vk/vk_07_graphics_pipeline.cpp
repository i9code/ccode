/*
Vulkan ͼ�ι��ߣ�ͨ�����������½ڣ����ǽ��Ὺ���й�ͼ�ι��ߵĻ��⣬ͨ����ͼ����Ⱦ���ߵ�����������������λ滭��
��νͼ�ι��߾��ǽ�meshʹ�õ���vertices�������ݺ���ͼ���ݣ�ת��Ϊ��Ⱦtargets���صĲ������С���Ҫ�ĸ�����ͼvk_07_img��ʾ:


Input assembler�ռ���ԭʼ�Ķ������ݣ����һ�����ʹ������������������Щ����Ԫ�أ������ظ�������Ķ������ݸ�����

vertex shader��Ӧ����ÿһ���������ݣ�ͨ��Ӧ�ñ任�������Ӷ��������λ���������ݴ�ģ�Ϳռ�ת��Ϊ��Ļ�ռ䡣�ڴ������֮��Ὣ���ݼ����ڹ��ߴ�����ȥ��

tessellation shader������ɫ���������ĳЩ�����һ��ϸ�ּ��������������������������Ӧ����ǽ���¥�ݱ��棬ʹ����������������ôƽ̹��

geometry shader������ɫ��Ӧ����ÿһ��ͼԪ�ϣ�����������ͼԪ������������ɫ�����ƣ���������
Ȼ�������ڽ����Ӧ���в�û��̫���Ӧ���龰����Ϊ����Intel�ļ����Կ��⣬������Կ�����֧�ֶ������롣

��դ���׶ν�ͼԪ�ֽ�ΪƬԪ����Щ�������֡�������ϵ�����Ԫ�ء���Ļ�������ƬԪ�ᱻ������������ɫ������������ڴ��ݵ�ƬԪ��ɫ���Ĺ����л�����ڲ�ֵ
��ͼ��ʾ������֮�⣬������Ȳ��ԵĽ��Ҳ���ƬԪ���ж�����

fragment shaderӦ����ÿ��ƬԪ��ȷ��ÿ��֡��������д���ƬԪ���ݵ���ɫ�����ֵ��ƬԪ��ɫ������ʹ�ö�����ɫ���Ĳ�ֵ���ݣ���ͼ��UV����͹�Դ�������ݡ�

��ɫ�����׶λ�Բ�ͬƬԪ����ɫ���л�ϣ�����ӳ�䵽֡��������ͬһ�������ϡ�ƬԪҲ���Լ򵥵Ļ�����д�����ӻ��߸���͸���Ȼ�ϡ�

��ɫ�Ľ׶α���Ϊ�̶���ˮ�ߡ�����׶�����ʹ���Զ���Ĳ�����ֵ���������ڲ��Ĺ����߼���Ԥ�ƺõġ�

��ɫ�Ľ׶α���Ϊ�ɱ�̽׶�programmable�����ǿ�����GPU�ύ�Լ���д�Ĵ���ִ�о�����߼���
������ʹ��fragment shader��ʱ�򣬿���ʵ���κδ���ͼ���ƹ�Ĺ���׷�١���Щ����ͬʱ�����ڶ��GPU�ϴ��������󣬱��粢�еĶ����Ƭ�γ���

���֮ǰʹ�ù��ɵ�API(OpenGL��Direct3D),��ô����������ͨ��glBlendFunc��OMSetBlendState���ø��Ĺ������á�
Vulkan�е�ͼ�ι��߼������ɸı䣬��������Ҫ������ɫ�����󶨵���ͬ��֡���������߸��Ļ�Ϻ�����������ͷ�������ߡ�
ȱ���Ǳ��봴��һЩ���ߣ���Щ���ߴ�������Ⱦ������ʹ�õĲ�ͬ�����״̬�������������й��ߵĲ���������ǰ֪���ģ����Կ���ͨ������������õ��Ż�����
*/
#include "vk_00_main.h"
void HelloTriangleApplication::createGraphicsPipeline() {
	auto vertShaderCode = readFile("src/vk/shaders/vert.spv");
	auto fragShaderCode = readFile("src/vk/shaders/frag.spv");

	/*
	shader module�����������Ⱦ���ߴ����������Ҫ���������ǻ���createGraphicsPipeline�����ж��屾�ر����������ǣ�
	�����Ƕ������Ա�����������ǵľ��:
	*/ 
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);


	//VkShaderModule����ֻ���ֽ��뻺������һ����װ��������ɫ����û�б˴����ӣ�����û�и���Ŀ�ġ�
	//ͨ��VkPipelineShaderStageCreateInfo�ṹ����ɫ��ģ����䵽�����еĶ������Ƭ����ɫ���׶Ρ�VkPipelineShaderStageCreateInfo�ṹ����ʵ�ʹ��ߴ������̵�һ���֡�

	//����������createGraphicsPipeline��������д������ɫ���ṹ�塣
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;  // ��һ����Ҫ��֪Vulkan�����ĸ���ˮ�߽׶�ʹ����ɫ��
	// ��������������Աָ�������������ɫ��ģ��͵��õ�������������ζ�ſ��Խ����Ƭ����ɫ����ϵ�������ɫ��ģ���У�
	//��ʹ�ò�ͬ����ڵ����������ǵ���Ϊ������������£����Ǽ��ʹ�ñ�׼main������Ϊ��ڡ�
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	/*
	����һ����ѡ��Ա��pSpecializationInfo,���������ǲ���ʹ����������ֵ������һ�¡�������Ϊ��ɫ��ָ������ֵ��
	ʹ�õ�����ɫ��ģ�飬ͨ��Ϊ����ʹ�ò�ͬ�ĳ���ֵ����������ˮ�ߴ���ʱ����Ϊ�������á�
	�������Ⱦʱʹ�ñ���������ɫ������Ч�ʣ���Ϊ���������Խ����Ż�����������ifֵ�жϵ���䡣
	���û�������ĳ��������Խ���Ա����Ϊnullptr�����ǵ�struct�ṹ���ʼ���Զ����С�
	*/
	// �޸Ľṹ������Ƭ����ɫ������Ҫ:
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	// ��������ṹ��Ĵ�������ͨ�����鱣�棬�ⲿ�����ý�����ʵ�ʵĹ��ߴ�����ʼ��
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	//��������
	/*
	VkPipelineVertexInputStateCreateInfo�ṹ�������˶������ݵĸ�ʽ���ýṹ�����ݴ��ݵ�vertex shader�С��������ַ�ʽ��������:

	Bindings:�������ݵļ�϶��ȷ��������ÿ�����������ÿ��instance(instancing)
	Attribute ����:������Ҫ���а󶨼��������ԵĶ�����ɫ���е�����������͡�
	��Ϊ���ǽ���������Ӳ���뵽vertex shader�У��������ǽ�Ҫ���Ľṹ��û�ж�������ȥ���ء����ǽ�����vertex buffer�½��л���������
	*/
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// �������
	/*
	VkPipelineInputAssemblyStateCreateInfo�ṹ��������������:����������ʲô���͵ļ���ͼԪ���˽��л��Ƽ��Ƿ����ö��������¿�ʼͼԪ��ͼԪ�����˽ṹ����topologyö��ֵ����:

	VK_PRIMITIVE_TOPOLOGY_POINT_LIST: ���㵽��
	VK_PRIMITIVE_TOPOLOGY_LINE_LIST: ������ߣ����㲻����
	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: ������ߣ�ÿ���߶εĽ���������Ϊ��һ���߶εĿ�ʼ����
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: ������棬���㲻����
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: ÿ������ѵ�ĵڶ��������������㶼��Ϊ��һ�������ε�ǰ��������
	��������£��������ݰ��ջ������е�������Ϊ����������Ҳ����ͨ��element buffer����������ָ���������ݵ�������ͨ�����ö��������������ܡ��������primitiveRestartEnable��ԱΪVK_TRUE������ͨ��0xFFFF����0xFFFFFFFF��Ϊ�����������ֽ��ߺ���������_STRIPģʽ�µ�ͼԪ���˽ṹ��

	ͨ�����̳̻��������Σ��������Ǽ�ְ������¸�ʽ������ݽṹ:
	*/
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	
	/*
	�Ӵ��Ͳü�
	Viewport��������framebuffer��Ϊ��Ⱦ������Ŀ������������ֵ�ڱ��̳�������������(0, 0)��(width, height)
	*/
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	/*
	�ǵý�����������imagesͼ���СWIDTH��HEIGHT����ݲ�ͬ�Ĵ������ͬ��������ͼ�񽫻���֡������framebuffersʹ�ã���������Ӧ�ü�����ǵĴ�С��

	minDepth��maxDepth��ֵָ��framebuffer����ȵķ�Χ����Щ��ֵ����������[0.0f, 1.0f]����壬����minDepth���ܻ����maxDepth��
	����㲻���κ�ָ��������ʹ�ñ�׼����ֵ0.0f��1.0f��

	viewports������imageͼ��framebuffer֡��������ת����ϵ���ü����ζ�������Щ��������ر��洢��
	�κ��ڲü�����������ض����ڹ�դ���׶ζ��������ǵĹ��ܸ�������������Ƕ���ת����ϵ��
	�����������ͼ��ʾ����Ҫע����ǣ�����ͼ���viewport�ߴ������Σ����Ĳü�����ֻ���ڶ���ܵ�һ�����֡�
	*/

	//��������Ҫ��ͼ����Ƶ�������֡������framebuffer�У��������Ƕ���ü����θ��ǵ�����ͼ��:
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	//viewport�Ͳü�������Ҫ����VkPipelineViewportStateCreateInfo�ṹ������ʹ�á�����ʹ�ö�viewports�Ͳü�������һЩͼ�ο���ͨ���������á�
	//ʹ�ø�������ҪGPU֧�ָù��ܣ����忴�߼��豸�Ĵ�����
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// ��դ��
	/*
	��դ��ͨ��������ɫ��������ļ����㷨������������Σ�����ͼ�δ��ݵ�Ƭ����ɫ��������ɫ������
	��Ҳ��ִ����Ȳ���depth testing�������face culling�Ͳü����ԣ������Զ������ƬԪ�������ã������Ƿ��������ͼԪ���˻����Ǳ߿�(�߿���Ⱦ)��
	���е�����ͨ��VkPipelineRasterizationStateCreateInfo�ṹ�嶨�塣
	*/
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE; // ����depthClampEnable����ΪVK_TRUE������Զ���ü����ƬԪ����������������Ƕ������ǡ��������������±Ƚ����ã�����Ӱ��ͼ��ʹ�øù�����Ҫ�õ�GPU��֧�֡�

	rasterizer.rasterizerDiscardEnable = VK_FALSE;  //���rasterizerDiscardEnable����ΪVK_TRUE����ô����ͼԪ��Զ���ᴫ�ݵ���դ���׶Ρ����ǻ����Ľ�ֹ�κ������framebuffer֡�������ķ�����
	
	/*
	polygonMode�������β���ͼƬ�����ݡ�������Чģʽ:

	VK_POLYGON_MODE_FILL: ������������
	VK_POLYGON_MODE_LINE: ����α�Ե�߿����
	VK_POLYGON_MODE_POINT: ����ζ�����Ϊ������
	ʹ���κ�ģʽ�����Ҫ����GPU���ܡ�
	*/
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	
	rasterizer.lineWidth = 1.0f;/// lineWidth��Ա��ֱ�����ģ�����ƬԪ�����������ߵĿ�ȡ������߿�֧��ȡ����Ӳ�����κδ���1.0���߿���Ҫ����GPU��wideLines����֧�֡�

	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; // cullMode�������ھ�����ü������ͷ�ʽ�����Խ�ֹculling���ü�front faces��cull back faces ����ȫ��
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // frontFace����������Ϊfront-facing��Ķ����˳�򣬿�����˳ʱ��Ҳ��������ʱ�롣
	rasterizer.depthBiasEnable = VK_FALSE; // ��դ������ͨ����ӳ������߻���ƬԪ��б�����������ֵ��һЩʱ�������Ӱ��ͼ�����õģ��������ǲ������½���ʹ�ã�����depthBiasEnableΪVK_FALSE��

	// �ز���
	/*
	VkPipelineMultisampleStateCreateInfo�ṹ���������ö��ز�������ν���ز����ǿ����anti-aliasing��һ��ʵ�֡���ͨ����϶������ε�Ƭ����ɫ���������դ����ͬһ�����ء�
	����Ҫ�����ڱ�Ե����Ҳ��������עĿ�ľ�ݳ��ֵĵط������ֻ��һ�������ӳ�䵽�����ǲ���Ҫ�������Ƭ����ɫ�����в����ģ���ȸ߷ֱ�����˵�����Ứ�ѽϵ͵Ŀ����������ù�����ҪGPU֧��
	*/
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	/*
	��Ⱥ�ģ�����
	���ʹ��depth ���� stencil����������Ҫʹ��VkPipelineDepthStencilStateCreateInfo���á��������ڲ���Ҫʹ�ã����Լ򵥵Ĵ���nullptr�������ⲿ�ֻ�ר������Ȼ������½������ۡ�
	*/
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	/*
	��ɫ���
	Ƭ����ɫ������������ɫ������Ҫ��֡������framebuffer���Ѿ����ڵ���ɫ���л�ϡ����ת���Ĺ��̳�Ϊ��ɫ���������ַ�ʽ:

	��old��new��ɫ���л�ϲ���һ�����յ���ɫ
	ʹ�ð�λ�������old��new��ɫ��ֵ
	�������ṹ������������ɫ��ϡ���һ���ṹ��VkPipelineColorBlendAttachmentState������ÿ�����ӵ�֡�����������á��ڶ����ṹ��VkPipelineColorBlendStateCreateInfo������ȫ�ֻ�ɫ�����á������ǵ������н�ʹ�õ�һ�ַ�ʽ:
	*/
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;



	/*��������ɫ����ʹ��uniform�����������붯̬״̬������ȫ�ֱ����������ڻ滭ʱ�޸ģ����Ը�����ɫ������Ϊ���������´������ǡ�����ͨ�����ڽ��任���󴫵ݵ�������ɫ��������Ƭ����ɫ���崴�������������

	��Щuniform��ֵ��Ҫ�ڹ��ߴ��������У�ͨ��VkPipelineLayout����ָ������ʹ�ں����������õ�������Ҳ��Ȼ��Ҫ����һ���յ�pipeline layout��
	�������Ա�������иö�����Ϊ�����ں����½��еĺ����������� :
	*/
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	//��ͼ�ι��ߴ��������createGraphicsPipeline�������ص�ʱ������Ӧ�ñ�������������ڸú�����ɾ������:
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}