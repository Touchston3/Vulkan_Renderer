#include "Device.hpp"


#include <cstdint>
#include <vulkan/vulkan_core.h>
#include "PhysicalDevice.hpp"

using namespace std;

namespace gfx {
	Device::Device( PhysicalDevice& physical_device, const vector<const char*>& validation_layers, Device::Extensions& extensions ) :
		_queue{},
		_device{}
	{

		bool has_graphics_queue_support = false;
		uint32_t graphics_family_queue = 0;
		
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties( physical_device.get(), &queue_family_count, nullptr );
		auto queue_family_properties = vector<VkQueueFamilyProperties>{ queue_family_count };
		vkGetPhysicalDeviceQueueFamilyProperties( physical_device.get(), &queue_family_count, queue_family_properties.data() );
		

		int i = 0;
		for( auto queue_family : queue_family_properties ) {
			if( queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {	
				has_graphics_queue_support = true;
				graphics_family_queue = i;
			}
			i++;
		}

		//setup logical device and graphics queue
		float queue_priority = 1.f;
		uint32_t queue_create_info_count = 1;
		auto queue_create_info = VkDeviceQueueCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.flags = {},
			.queueFamilyIndex = graphics_family_queue,
			.queueCount = 1,
			.pQueuePriorities = &queue_priority,
		};

		auto device_features = VkPhysicalDeviceFeatures{};
		auto device_info = VkDeviceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.flags = {},
			.queueCreateInfoCount = queue_create_info_count,
			.pQueueCreateInfos = &queue_create_info,
			.enabledLayerCount = static_cast<uint32_t>(validation_layers.size()),
			.ppEnabledLayerNames = validation_layers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(extensions.get().size()), 
			.ppEnabledExtensionNames = extensions.get().data(),
			.pEnabledFeatures = &device_features
		};

		vkCreateDevice( 
			physical_device.get(),
			&device_info,
			nullptr,
			&_device 
		);

		vkGetDeviceQueue(
			this->get(),
			graphics_family_queue,
			0,
			&_queue 
		);	
	}
	Device::~Device() {
		vkDestroyDevice( _device, nullptr );
	}
}
