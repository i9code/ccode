/*
Vulkan 图形管线，通过接下来的章节，我们将会开启有关图形管线的话题，通过对图形渲染管线的配置完成最后的三角形绘画。
所谓图形管线就是将mesh使用到的vertices定点数据和贴图数据，转化为渲染targets像素的操作序列。简要的概述如图vk_07_img所示:


Input assembler收集最原始的顶点数据，并且还可以使用索引缓冲区复用这些数据元素，而不必复制冗余的顶点数据副本。

vertex shader会应用在每一个顶点数据，通常应用变换操作，从而将顶点的位置坐标数据从模型空间转换为屏幕空间。在处理完毕之后会将数据继续在管线传递下去。

tessellation shader曲面着色器允许基于某些规则进一步细分几何数据增加网格的质量。经常应用与墙面和楼梯表面，使附近看起来不是那么平坦。

geometry shader集合着色器应用在每一个图元上，可用于增减图元，它与曲面着色器类似，但更加灵活。
然而，它在今天的应用中并没有太多的应用情景，因为除了Intel的集成显卡外，大多数显卡性能支持都不理想。

光栅化阶段将图元分解为片元。这些是填充在帧缓冲区上的像素元素。屏幕外区域的片元会被丢弃，顶点着色器输出的数据在传递到片元着色器的过程中会进行内插值
如图所示。除此之外，根据深度测试的结果也会对片元进行丢弃。

fragment shader应用于每个片元，确定每个帧缓冲区中写入的片元数据的颜色和深度值。片元着色器可以使用顶点着色器的插值数据，贴图的UV坐标和光源法线数据。

混色操作阶段会对不同片元的颜色进行混合，最终映射到帧缓冲区的同一个像素上。片元也可以简单的互相重写，叠加或者根据透明度混合。

绿色的阶段被称为固定流水线。这个阶段允许使用自定义的参数数值，但是它内部的工作逻辑是预制好的。

橙色的阶段被称为可编程阶段programmable，我们可以向GPU提交自己编写的代码执行具体的逻辑。
比如在使用fragment shader的时候，可以实现任何从贴图到灯光的光线追踪。这些程序同时运行在多核GPU上处理许多对象，比如并行的顶点和片段程序。

如果之前使用过旧的API(OpenGL和Direct3D),那么将可以随意通过glBlendFunc和OMSetBlendState调用更改管线设置。
Vulkan中的图形管线几乎不可改变，因此如果需要更改着色器，绑定到不同的帧缓冲区或者更改混合函数，则必须从头创建管线。
缺点是必须创建一些管线，这些管线代表在渲染操作中使用的不同的组合状态。但是由于所有管线的操作都是提前知道的，所以可以通过驱动程序更好的优化它。
*/
#include "vk_00_main.h"
void HelloTriangleApplication::createGraphicsPipeline() {
	auto vertShaderCode = readFile("src/vk/shaders/vert.spv");
	auto fragShaderCode = readFile("src/vk/shaders/frag.spv");

	/*
	shader module对象仅仅在渲染管线处理过程中需要，所以我们会在createGraphicsPipeline函数中定义本地变量保存它们，
	而不是定义类成员变量持有它们的句柄:
	*/ 
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);


	//VkShaderModule对象只是字节码缓冲区的一个包装容器。着色器并没有彼此链接，甚至没有给出目的。
	//通过VkPipelineShaderStageCreateInfo结构将着色器模块分配到管线中的顶点或者片段着色器阶段。VkPipelineShaderStageCreateInfo结构体是实际管线创建过程的一部分。

	//我们首先在createGraphicsPipeline函数中填写顶点着色器结构体。
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;  // 第一个需要告知Vulkan将在哪个流水线阶段使用着色器
	// 接下来的两个成员指定包含代码的着色器模块和调用的主函数。这意味着可以将多个片段着色器组合到单个着色器模块中，
	//并使用不同的入口点来区分它们的行为。在这种情况下，我们坚持使用标准main函数作为入口。
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	/*
	还有一个可选成员，pSpecializationInfo,在这里我们不会使用它，但是值得讨论一下。它允许为着色器指定常量值。
	使用单个着色器模块，通过为其中使用不同的常量值，可以在流水线创建时对行为进行配置。
	这比在渲染时使用变量配置着色器更有效率，因为编译器可以进行优化，例如消除if值判断的语句。
	如果没有这样的常量，可以将成员设置为nullptr，我们的struct结构体初始化自动进行。
	*/
	// 修改结构体满足片段着色器的需要:
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	// 完成两个结构体的创建，并通过数组保存，这部分引用将会在实际的管线创建开始。
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	//顶点输入
	/*
	VkPipelineVertexInputStateCreateInfo结构体描述了顶点数据的格式，该结构体数据传递到vertex shader中。它以两种方式进行描述:

	Bindings:根据数据的间隙，确定数据是每个顶点或者是每个instance(instancing)
	Attribute 描述:描述将要进行绑定及加载属性的顶点着色器中的相关属性类型。
	因为我们将顶点数据硬编码到vertex shader中，所以我们将要填充的结构体没有顶点数据去加载。我们将会在vertex buffer章节中回来操作。
	*/
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// 输入组件
	/*
	VkPipelineInputAssemblyStateCreateInfo结构体描述两件事情:顶点数据以什么类型的几何图元拓扑进行绘制及是否启用顶点索重新开始图元。图元的拓扑结构类型topology枚举值如下:

	VK_PRIMITIVE_TOPOLOGY_POINT_LIST: 顶点到点
	VK_PRIMITIVE_TOPOLOGY_LINE_LIST: 两点成线，顶点不共用
	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: 两点成线，每个线段的结束顶点作为下一个线段的开始顶点
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: 三点成面，顶点不共用
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: 每个但教训的第二个、第三个顶点都作为下一个三角形的前两个顶点
	正常情况下，顶点数据按照缓冲区中的序列作为索引，但是也可以通过element buffer缓冲区自行指定顶点数据的索引。通过复用顶点数据提升性能。如果设置primitiveRestartEnable成员为VK_TRUE，可以通过0xFFFF或者0xFFFFFFFF作为特殊索引来分解线和三角形在_STRIP模式下的图元拓扑结构。

	通过本教程绘制三角形，所以我们坚持按照如下格式填充数据结构:
	*/
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	
	/*
	视窗和裁剪
	Viewport用于描述framebuffer作为渲染输出结果目标区域。它的数值在本教程中总是设置在(0, 0)和(width, height)
	*/
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	/*
	记得交换链和它的images图像大小WIDTH和HEIGHT会根据不同的窗体而不同。交换链图像将会在帧缓冲区framebuffers使用，所以我们应该坚持它们的大小。

	minDepth和maxDepth数值指定framebuffer中深度的范围。这些数值必须收敛在[0.0f, 1.0f]区间冲，但是minDepth可能会大于maxDepth。
	如果你不做任何指定，建议使用标准的数值0.0f和1.0f。

	viewports定义了image图像到framebuffer帧缓冲区的转换关系，裁剪矩形定义了哪些区域的像素被存储。
	任何在裁剪巨型外的像素都会在光栅化阶段丢弃。它们的功能更像过滤器而不是定义转换关系。
	这个区别如下图所示。需要注意的是，对于图像比viewport尺寸大的情形，左侧的裁剪矩形只是众多可能的一个表现。
	*/

	//中我们需要将图像绘制到完整的帧缓冲区framebuffer中，所以我们定义裁剪矩形覆盖到整体图像:
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	//viewport和裁剪矩形需要借助VkPipelineViewportStateCreateInfo结构体联合使用。可以使用多viewports和裁剪矩形在一些图形卡，通过数组引用。
	//使用该特性需要GPU支持该功能，具体看逻辑设备的创建。
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// 光栅化
	/*
	光栅化通过顶点着色器及具体的几何算法将顶点进行塑形，并将图形传递到片段着色器进行着色工作。
	它也会执行深度测试depth testing、面裁切face culling和裁剪测试，它可以对输出的片元进行配置，决定是否输出整个图元拓扑或者是边框(线框渲染)。
	所有的配置通过VkPipelineRasterizationStateCreateInfo结构体定义。
	*/
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE; // 它的depthClampEnable设置为VK_TRUE，超过远近裁剪面的片元会进行收敛，而不是丢弃它们。它在特殊的情况下比较有用，像阴影贴图。使用该功能需要得到GPU的支持。

	rasterizer.rasterizerDiscardEnable = VK_FALSE;  //如果rasterizerDiscardEnable设置为VK_TRUE，那么几何图元永远不会传递到光栅化阶段。这是基本的禁止任何输出到framebuffer帧缓冲区的方法。
	
	/*
	polygonMode决定几何产生图片的内容。下列有效模式:

	VK_POLYGON_MODE_FILL: 多边形区域填充
	VK_POLYGON_MODE_LINE: 多边形边缘线框绘制
	VK_POLYGON_MODE_POINT: 多边形顶点作为描点绘制
	使用任何模式填充需要开启GPU功能。
	*/
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	
	rasterizer.lineWidth = 1.0f;/// lineWidth成员是直接填充的，根据片元的数量描述线的宽度。最大的线宽支持取决于硬件，任何大于1.0的线宽需要开启GPU的wideLines特性支持。

	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; // cullMode变量用于决定面裁剪的类型方式。可以禁止culling，裁剪front faces，cull back faces 或者全部
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // frontFace用于描述作为front-facing面的顶点的顺序，可以是顺时针也可以是逆时针。
	rasterizer.depthBiasEnable = VK_FALSE; // 光栅化可以通过添加常量或者基于片元的斜率来更改深度值。一些时候对于阴影贴图是有用的，但是我们不会在章节中使用，设置depthBiasEnable为VK_FALSE。

	// 重采样
	/*
	VkPipelineMultisampleStateCreateInfo结构体用于配置多重采样。所谓多重采样是抗锯齿anti-aliasing的一种实现。它通过组合多个多边形的片段着色器结果，光栅化到同一个像素。
	这主要发生在边缘，这也是最引人注目的锯齿出现的地方。如果只有一个多边形映射到像素是不需要多次运行片段着色器进行采样的，相比高分辨率来说，它会花费较低的开销。开启该功能需要GPU支持
	*/
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	/*
	深度和模板测试
	如果使用depth 或者 stencil缓冲区，需要使用VkPipelineDepthStencilStateCreateInfo配置。我们现在不需要使用，所以简单的传递nullptr，关于这部分会专门在深度缓冲区章节中讨论。
	*/
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	/*
	颜色混合
	片段着色器输出具体的颜色，它需要与帧缓冲区framebuffer中已经存在的颜色进行混合。这个转换的过程成为混色，它有两种方式:

	将old和new颜色进行混合产出一个最终的颜色
	使用按位操作混合old和new颜色的值
	有两个结构体用于配置颜色混合。第一个结构体VkPipelineColorBlendAttachmentState包括了每个附加到帧缓冲区的配置。第二个结构体VkPipelineColorBlendStateCreateInfo包含了全局混色的设置。在我们的例子中仅使用第一种方式:
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



	/*可以在着色器中使用uniform，它是类似与动态状态变量的全局变量，可以在绘画时修改，可以更改着色器的行为而无需重新创建它们。它们通常用于将变换矩阵传递到顶点着色器或者在片段着色器冲创建纹理采样器。

	这些uniform数值需要在管线创建过程中，通过VkPipelineLayout对象指定。即使在后续内容中用到，我们也仍然需要创建一个空的pipeline layout。
	创建类成员变量持有该对象，因为我们在后续章节中的函数中引用它 :
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

	//在图形管线创建完成且createGraphicsPipeline函数返回的时候，它们应该被清理掉，所以在该函数后删除它们:
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}