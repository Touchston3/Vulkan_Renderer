#include <vulkan/vulkan_core.h>
#include "../utils/Exception.hpp"
#include "Instance.hpp"

using namespace std;

namespace gfx {
	Instance::Instance( const Window& window, const vector<const char*>& validation_layers ) :
		_instance{}
	{
		auto application_info = VkApplicationInfo {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Vulkan Renderer",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "Test Engine",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = VK_API_VERSION_1_3,
		};

		auto instance_info = VkInstanceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.flags = {},
			.pApplicationInfo = &application_info,
			.enabledLayerCount = static_cast<uint32_t>( validation_layers.size() ), //Validation Layers
			.ppEnabledLayerNames = validation_layers.data(), //Validation Layers
			.enabledExtensionCount = static_cast<uint32_t>( window._extensions.size() ), //Extensions
			.ppEnabledExtensionNames = window._extensions.data(), //Extensions
		};

		auto create_instance_result = (VkResult) vkCreateInstance( &instance_info, nullptr, &_instance );
		if( create_instance_result != VkResult::VK_SUCCESS )
			throw Utils::Exception<int>{ "Create Vulkan Instance Issue" };
	}

	Instance::~Instance(){ vkDestroyInstance( this->get(), nullptr ); }
}
