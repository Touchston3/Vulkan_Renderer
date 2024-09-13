#pragma once

#include <vulkan/vulkan.h>

#include "Device.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"
namespace gfx
{
	class SwapChain 
	{
		public:
			SwapChain(Device* device, PhysicalDevice& physical_device, Surface& surface);
			~SwapChain();
	
			inline VkSwapchainKHR& get() { return _swap_chain; }
		private:
			VkSwapchainKHR _swap_chain;	
			Device* _device;
	};
}
