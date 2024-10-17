#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include "PhysicalDevice.hpp"

namespace gfx {
	class Device {

		public:
			class Extensions {
				public:
					Extensions( const std::vector<const char*>& required_extensions );
					inline std::vector<const char*>& get() { return _extensions; }

				private:
					std::vector<const char*> _extensions; 
			};

		public:
			Device( PhysicalDevice& physical_device, const std::vector<const char*>& validation_layers, Device::Extensions& extensions );
			~Device();

			inline VkDevice& get() { return _device; }

			VkQueue _queue;

		private:
			VkDevice _device;
	};
}
