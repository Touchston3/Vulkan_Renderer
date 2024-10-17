#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "Device.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"
#include "Window.hpp"
namespace gfx {
	class SwapChain {
		public:

			SwapChain(Device* device, PhysicalDevice& physical_device, Surface& surface, Window& window);
			~SwapChain();
	
			inline VkSwapchainKHR& get() { return _swap_chain; }
			std::vector<VkImage> _images;
			std::vector<VkImageView> _image_views;

		private:

			VkSwapchainKHR _swap_chain;	
			Device* _device;
	};
}
