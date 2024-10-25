#pragma once

#include "Device.hpp"
#include "SwapChain.hpp"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace gfx {
	class Pipeline {
		public:
			Pipeline(Device& device, SwapChain& swapchain);
			~Pipeline();

			VkShaderModule _vert_shader_module;
			VkShaderModule _frag_shader_module;
		private:
			Device& _device;
	};
}
