#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include "PhysicalDevice.hpp"

namespace gfx
{
	class Device
	{
		public:
			Device(PhysicalDevice& physical_device, const std::vector<const char*>& validation_layers, const std::vector<const char*>& device_extesions);
			~Device();

			inline VkDevice& get() { return _device; }

			VkQueue _queue;
		private:
			VkDevice _device;
	};
}
