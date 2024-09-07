#pragma once

#include <vulkan/vulkan.h>
#include "Instance.hpp"

namespace gfx
{
	class PhysicalDevice
	{
		public: 
			PhysicalDevice( Instance& vulkan_instance );
			~PhysicalDevice();
			
			inline VkPhysicalDevice& get() { return _physical_device; }

		private:
			VkPhysicalDevice _physical_device;
	};
}
