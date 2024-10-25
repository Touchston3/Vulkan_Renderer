#include "Pipeline.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../utils/utils.hpp"
#include "SwapChain.hpp"

namespace gfx {

	Pipeline::Pipeline(Device& device, SwapChain& swapchain) :
		_vert_shader_module{},
		_frag_shader_module{},
		_device{ device }
	{
		{
			auto vert_shader_binaries = (std::vector<char>) Utils::load_shader("../resources/shaders/bin/test.vert.spv");
			auto frag_shader_binaries = (std::vector<char>) Utils::load_shader("../resources/shaders/bin/test.frag.spv");

			auto vert_shader_module_info = VkShaderModuleCreateInfo {
				.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.codeSize = vert_shader_binaries.size(),
				.pCode = reinterpret_cast<const uint32_t*>( vert_shader_binaries.data() ),
			};
			auto frag_shader_module_info = VkShaderModuleCreateInfo {
				.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.codeSize = frag_shader_binaries.size(),
				.pCode = reinterpret_cast<const uint32_t*>( frag_shader_binaries.data() ),
			};


			vkCreateShaderModule(
				device.get(),
				&vert_shader_module_info, 
				nullptr, 
				&_vert_shader_module
			);
			vkCreateShaderModule(
				device.get(),
				&frag_shader_module_info, 
				nullptr, 
				&_frag_shader_module
			);
		}

		{
			auto vert_shader_stage_info = VkPipelineShaderStageCreateInfo {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_VERTEX_BIT,
				.module = _vert_shader_module,
				.pName = "main",
			};
			auto frag_shader_stage_info = VkPipelineShaderStageCreateInfo {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = _frag_shader_module,
				.pName = "main",
			};
		}

		{
			auto vertex_input_info = VkPipelineVertexInputStateCreateInfo {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.vertexBindingDescriptionCount = 0,
				.pVertexBindingDescriptions = nullptr,
				.vertexAttributeDescriptionCount = 0,
				.pVertexAttributeDescriptions = nullptr,
			};
			
			auto input_assembly_info = VkPipelineInputAssemblyStateCreateInfo {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.primitiveRestartEnable = VK_FALSE,
			};
			auto viewport = VkViewport {
				.x = 0.0f, 
				.y = 0.0f, 
				.width = static_cast<float>( swapchain._image_extent.width ),
				.height = static_cast<float>( swapchain._image_extent.height ),
				.minDepth = 0.0f,
				.maxDepth = 1.0f,
			};
			auto scissor = VkRect2D {
				.offset = { 0, 0 },
				.extent = swapchain._image_extent,
			};

			auto dynamic_state_list = std::vector<VkDynamicState> {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR,
			};	
			auto dynamic_states_info = VkPipelineDynamicStateCreateInfo {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, 
				.dynamicStateCount = static_cast<uint32_t>( dynamic_state_list.size() ),
				.pDynamicStates = dynamic_state_list.data(),
			};
			
			auto viewport_state_info = VkPipelineViewportStateCreateInfo {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				.viewportCount = 1,
				.pViewports = &viewport,
				.scissorCount = 1,
				.pScissors = &scissor,
			};
		}
		auto rasterizer_info = VkPipelineRasterizationStateCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthClampEnable = VK_FALSE,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.rasterizerDiscardEnable = VK_FALSE,	
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.lineWidth = 1.0f,
		};
		auto multisampling_info = VkPipelineMultisampleStateCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE,
		};	
		//DEPTH AND STENCIL TESTTTTT!!!!!
	}
	Pipeline::~Pipeline() {
		vkDestroyShaderModule(_device.get(), _vert_shader_module, nullptr);
		vkDestroyShaderModule(_device.get(), _frag_shader_module, nullptr);
	}
}
