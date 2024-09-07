#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Window.hpp"

namespace gfx
{
	class Instance
	{
		public: 
			Instance(const Window& window, const std::vector<const char*>& validation_layers);
			~Instance();

			inline VkInstance& get() { return _instance; }
		private:
			VkInstance _instance;			

	};
}
