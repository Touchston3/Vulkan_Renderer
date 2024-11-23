#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "Instance.hpp"

namespace gfx
{
	class PhysicalDevice
	{
		public: 
			PhysicalDevice( Instance& vulkan_instance );
			~PhysicalDevice();
			
			inline VkPhysicalDevice& get() { return _physical_device; }
			VkPhysicalDeviceProperties _properties;
			VkPhysicalDeviceFeatures _features;

		private:
			VkPhysicalDevice _physical_device;
	};
}
