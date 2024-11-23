#include "Pipeline.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../utils/Files.hpp"
#include "../utils/Exception.hpp"
#include "SwapChain.hpp"

namespace gfx {

	Pipeline::Pipeline(Device& device, SwapChain& swapchain) :
		_vert_shader_module{},
		_frag_shader_module{},
		_device{ device },
		_swapchain{ swapchain },
		_render_pass{},
		_pipeline_layout{},
		_framebuffers{ swapchain._image_views.size() },
		_command_pool{},
		_command_buffer{},
		_active_framebuffer_index{0},
		_viewport {
			.x = 0.0f, 
			.y = 0.0f, 
			.width = static_cast<float>( swapchain._image_extent.width ),
			.height = static_cast<float>( swapchain._image_extent.height ),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		},
		_scissor {
			.offset = { 0, 0 },
			.extent = swapchain._image_extent,
		},
		_image_available_semaphore{},
		_render_complete_semaphore{},
		_in_flight_fence{}
	{
		{
			auto vert_shader_binaries = (std::vector<char>) Utils::Files::load_shader("../resources/shaders/bin/test.vert.spv");
			auto frag_shader_binaries = (std::vector<char>) Utils::Files::load_shader("../resources/shaders/bin/test.frag.spv");

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
		auto shader_stage_info = std::vector<VkPipelineShaderStageCreateInfo> {
			vert_shader_stage_info,
			frag_shader_stage_info,
		};

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
			.pViewports = &_viewport,
			.scissorCount = 1,
			.pScissors = &_scissor,
		};

		auto rasterizer_info = VkPipelineRasterizationStateCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,	
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
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
	
		//Color blending for current framebuffer
		auto color_blend_attachment = VkPipelineColorBlendAttachmentState {
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
							  VK_COLOR_COMPONENT_B_BIT |
							  VK_COLOR_COMPONENT_B_BIT |
							  VK_COLOR_COMPONENT_A_BIT,
		};
		
		auto color_blend_info = VkPipelineColorBlendStateCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &color_blend_attachment,
		};
		auto color_attachment = VkAttachmentDescription {
			.format = swapchain._format.format,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		};
		auto color_attachment_reference = VkAttachmentReference {
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		};
		auto subpass = VkSubpassDescription {
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = 1,
			.pColorAttachments = &color_attachment_reference,	
		}; 
		auto subpass_dependency = VkSubpassDependency {
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		};

		auto render_pass_info = VkRenderPassCreateInfo {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = 1,
			.pAttachments = &color_attachment,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &subpass_dependency,
		};

		if( vkCreateRenderPass( device.get(), &render_pass_info, nullptr, &_render_pass ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue creating render pass" };
		}
		auto pipeline_layout_info = VkPipelineLayoutCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr,
		};

		if( vkCreatePipelineLayout( device.get(), &pipeline_layout_info, nullptr, &_pipeline_layout ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue creating pipeline layout" };
		}	
		
		auto pipeline_info = VkGraphicsPipelineCreateInfo {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount = static_cast<uint32_t>( shader_stage_info.size()) ,
			.pStages = shader_stage_info.data(),
			.pVertexInputState = &vertex_input_info,
			.pInputAssemblyState = &input_assembly_info,
			.pViewportState = &viewport_state_info,
			.pRasterizationState = &rasterizer_info,
			.pMultisampleState = &multisampling_info,
			.pDepthStencilState = nullptr,
			.pColorBlendState = &color_blend_info,
			.pDynamicState = &dynamic_states_info,
			.layout = _pipeline_layout,
			.renderPass = _render_pass, // DO THE RENDERPASS
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		};	
		
		if( vkCreateGraphicsPipelines( device.get(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &_pipeline ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue creating pipeline" };
		}
		
		for(uint32_t i = 0; i < swapchain._image_views.size(); i++ ) {
			auto framebuffer_attachments = std::vector<VkImageView>{
				swapchain._image_views[i],
			};

			auto framebuffer_info = VkFramebufferCreateInfo {
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass = _render_pass,
				.attachmentCount = static_cast<uint32_t>( framebuffer_attachments.size() ),
				.pAttachments = framebuffer_attachments.data(),
				.width = swapchain._image_extent.width,
				.height = swapchain._image_extent.height,
				.layers = 1,
			};
			
			if( vkCreateFramebuffer( device.get(), &framebuffer_info, nullptr, &_framebuffers[i]) != VK_SUCCESS ) {
				throw Utils::Exception<int>{ "Issue creating framebuffer" };
			}
		}
		float queue_priority = 1.0f;
		auto device_queue_info = VkDeviceQueueCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = device._graphics_family_queue_index,
			.queueCount = 1,
			.pQueuePriorities = &queue_priority,
		};	

		auto command_pool_info = VkCommandPoolCreateInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = device._graphics_family_queue_index,
		};
		if( vkCreateCommandPool( device.get(), &command_pool_info, nullptr, &_command_pool ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue creating command pool" };
		}

		auto command_buffer_allocation_info = VkCommandBufferAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = _command_pool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1,
		};
		if( vkAllocateCommandBuffers( device.get(), &command_buffer_allocation_info, &_command_buffer ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue allocating command buffer" };
		}

		
		auto semaphore_info = VkSemaphoreCreateInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};
		auto fence_info = VkFenceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT, //So it triggers the first time we draw the frame
		};
		if( vkCreateSemaphore( device.get(), &semaphore_info, nullptr, &_image_available_semaphore ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue creating image availabe semaphore" };
		}
		if( vkCreateSemaphore( device.get(), &semaphore_info, nullptr, &_render_complete_semaphore ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue creating render complete semaphore" };
		}
		if( vkCreateFence( device.get(), &fence_info, nullptr, &_in_flight_fence ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue creating in flight fence" };
		}
		
		
	}
	void Pipeline::begin_drawing() {
		//Record command buffer
	}
	
	void Pipeline::draw(){

		vkWaitForFences( _device.get(), 1, &_in_flight_fence, VK_TRUE, UINT64_MAX );
		vkResetFences( _device.get(), 1, &_in_flight_fence );
		
		uint32_t image_index;
		vkAcquireNextImageKHR(
			_device.get(),
			_swapchain.get(),
			UINT64_MAX,
			_image_available_semaphore,
			VK_NULL_HANDLE,
			&image_index 
		);

		vkResetCommandBuffer( _command_buffer, 0 );

		//Begin recording
		auto command_buffer_begin_recording_info = VkCommandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = 0,
			.pInheritanceInfo = nullptr,
		};
		if( vkBeginCommandBuffer( _command_buffer, &command_buffer_begin_recording_info ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue recording to command buffer" };
		}
		auto clear_color = VkClearValue {
				0.0f, 0.0f, 0.0f, 1.0f 
		};
		auto render_pass_begin_info = VkRenderPassBeginInfo {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = _render_pass,
			.framebuffer = _framebuffers[ _active_framebuffer_index ],
			.renderArea = VkRect2D {
				.offset = { 0, 0 },
				.extent = _swapchain._image_extent,
			},
			.clearValueCount = 1,
			.pClearValues = &clear_color,
		};

		vkCmdBeginRenderPass( _command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE );
		vkCmdBindPipeline( _command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline );
		vkCmdSetViewport( _command_buffer, 0, 1, &_viewport );
		vkCmdSetScissor( _command_buffer, 0, 1, &_scissor );
		vkCmdDraw( _command_buffer, 3, 1, 0, 0 );

		vkCmdEndRenderPass( _command_buffer );
		if( vkEndCommandBuffer( _command_buffer ) != VK_SUCCESS ) { 
			throw Utils::Exception<int>{ "Issue ending command buffer" };			
		}
		//end recording
	
		auto wait_semaphores = std::vector<VkSemaphore>{ _image_available_semaphore };
		auto wait_stages = std::vector<VkPipelineStageFlags>{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		auto signal_semaphores = std::vector<VkSemaphore>{ _render_complete_semaphore };
	
		auto command_buffer_submit_info = VkSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = static_cast<uint32_t>( wait_semaphores.size() ),
			.pWaitSemaphores = wait_semaphores.data(),
			.pWaitDstStageMask = wait_stages.data(),
			.commandBufferCount = 1,
			.pCommandBuffers = &_command_buffer,
			.signalSemaphoreCount = static_cast<uint32_t>( signal_semaphores.size() ),
			.pSignalSemaphores = signal_semaphores.data(),
		};
		
		if( vkQueueSubmit( _device._graphics_queue, 1, &command_buffer_submit_info, _in_flight_fence ) != VK_SUCCESS ) {
			throw Utils::Exception<int>{ "Issue submitting to graphics queue" };			
		}

		auto swapchains = std::vector<VkSwapchainKHR>{ _swapchain.get() };	

		auto presentation_info = VkPresentInfoKHR{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = static_cast<uint32_t>( signal_semaphores.size() ),
			.pWaitSemaphores = signal_semaphores.data(),
			.swapchainCount = static_cast<uint32_t>( swapchains.size() ),
			.pSwapchains = swapchains.data(),
			.pImageIndices = &image_index,
			.pResults = nullptr,
		};
		
		vkQueuePresentKHR( _device._present_queue, &presentation_info );

	}
	
	void Pipeline::end_drawing() {
	}

	Pipeline::~Pipeline() {
		vkDestroyShaderModule( _device.get(), _vert_shader_module, nullptr );
		vkDestroyShaderModule( _device.get(), _frag_shader_module, nullptr );

		vkDestroyRenderPass( _device.get(), _render_pass, nullptr );	

		for( auto framebuffer : _framebuffers ) {
			vkDestroyFramebuffer( _device.get(), framebuffer, nullptr );
		}
		vkDestroyCommandPool( _device.get(), _command_pool, nullptr );
		
		vkDestroySemaphore( _device.get(), _image_available_semaphore, nullptr );
		vkDestroySemaphore( _device.get(), _render_complete_semaphore, nullptr );
		vkDestroyFence( _device.get(), _in_flight_fence, nullptr );

		vkDestroyPipelineLayout( _device.get(), _pipeline_layout, nullptr );
		vkDestroyPipeline( _device.get(), _pipeline, nullptr );
	}
}
