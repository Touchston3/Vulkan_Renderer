#pragma once 
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "Instance.hpp"
#include "Window.hpp"
namespace gfx {
	class Surface {
		public:
			Surface( Instance* vulkan_instance, Window& window );
			~Surface();

			inline  VkSurfaceKHR& get()  { return _surface; }

		private:
			VkSurfaceKHR _surface;
			Instance* _vulkan_instance;
	};
}
