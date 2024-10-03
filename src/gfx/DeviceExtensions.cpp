#include "Device.hpp"
#include "iostream"
#include "../../vendor/glfw3/include/GLFW/glfw3.h"

namespace gfx
{
	Device::Extensions::Extensions(const std::vector<const char*>& required_extensions) :
		_extensions{std::vector<const char*>{0}} 
	{ 
		uint32_t glfw_required_extension_count = 0; 
		auto glfw_required_extensions = (const char**) glfwGetRequiredInstanceExtensions(&glfw_required_extension_count);

		_extensions = std::vector<const char*> {
			glfw_required_extensions,
			glfw_required_extensions + glfw_required_extension_count
		};
		_extensions.insert(
			_extensions.end(),
			required_extensions.begin(),
			required_extensions.end()
		);
		_extensions.resize(_extensions.size()); //Is this necessary?
	
		for( auto extension : _extensions ) {
			std::cout << extension << "\n";
		}

		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

		auto extension_properties = std::vector<VkExtensionProperties>{extension_count};
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties.data());

		//Iterate Extensions and check
	}
}

