#pragma once

#include "Device.hpp"
#include "SwapChain.hpp"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace gfx {
	class Pipeline {
		public:
			Pipeline(Device& device, SwapChain& swapchain);
			~Pipeline();

			void draw();
			void begin_drawing();
			void end_drawing();

			VkShaderModule _vert_shader_module;
			VkShaderModule _frag_shader_module;

		private:
			Device& _device;
			SwapChain& _swapchain;
			VkRenderPass _render_pass;
			VkPipelineLayout _pipeline_layout;
			VkPipeline _pipeline;	
			std::vector<VkFramebuffer> _framebuffers;
			VkCommandPool _command_pool;
			VkCommandBuffer _command_buffer;
			uint32_t _active_framebuffer_index;
			VkViewport _viewport;
			VkRect2D _scissor;	
			VkSemaphore _image_available_semaphore;
			VkSemaphore _render_complete_semaphore;
			VkFence _in_flight_fence;
	};
}
