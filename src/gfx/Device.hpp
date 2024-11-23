#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

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
			Device( PhysicalDevice& physical_device, Surface& surface, const std::vector<const char*>& validation_layers, Device::Extensions& extensions );
			~Device();

			inline VkDevice& get() { return _device; }

			VkQueue _graphics_queue;
			VkQueue _present_queue;	
			uint32_t _graphics_family_queue_index;
			uint32_t _present_family_queue_index;

		private:
			VkDevice _device;
	};
}
