#include "Device.hpp"


#include <cstdint>
#include <vulkan/vulkan_core.h>
#include "PhysicalDevice.hpp"

using namespace std;

namespace gfx {
	Device::Device( PhysicalDevice& physical_device, Surface& surface, const vector<const char*>& validation_layers, Device::Extensions& extensions ) :
		_graphics_queue{},
		_present_queue{},
		_graphics_family_queue_index{ 0 },
		_present_family_queue_index{ 0 },
		_device{}
	{

		bool has_graphics_queue_support = false;
		VkBool32 has_present_queue_support = false;
		
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties( physical_device.get(), &queue_family_count, nullptr );
		auto queue_family_properties = vector<VkQueueFamilyProperties>{ queue_family_count };
		vkGetPhysicalDeviceQueueFamilyProperties( physical_device.get(), &queue_family_count, queue_family_properties.data() );
		

		int i = 0;
		for( auto queue_family : queue_family_properties ) {
			if( queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {	
				has_graphics_queue_support = true;
				_graphics_family_queue_index = i;
			}
			i++;
		}
		
		vkGetPhysicalDeviceSurfaceSupportKHR( physical_device.get(), _present_family_queue_index, surface.get(), &has_present_queue_support );
		

		//setup logical device and graphics queue
		float graphics_queue_priority = 1.f;
		auto graphics_queue_create_info = VkDeviceQueueCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.flags = {},
			.queueFamilyIndex = _graphics_family_queue_index,
			.queueCount = 1,
			.pQueuePriorities = &graphics_queue_priority,
		};
		float present_queue_priority = 1.f;
		auto present_queue_create_info = VkDeviceQueueCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.flags = {},
			.queueFamilyIndex = _present_family_queue_index,
			.queueCount = 1,
			.pQueuePriorities = &present_queue_priority,
		};
		
		auto queue_create_infos = std::vector<VkDeviceQueueCreateInfo> {
			graphics_queue_create_info,
			present_queue_create_info,
		};	

		auto device_features = VkPhysicalDeviceFeatures{};
		auto device_info = VkDeviceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.flags = {},
			.queueCreateInfoCount = static_cast<uint32_t>( queue_create_infos.size() ),
			.pQueueCreateInfos = queue_create_infos.data(),
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
			_device,
			_graphics_family_queue_index,
			0,
			&_graphics_queue 
		);	
		vkGetDeviceQueue(
			_device,
			_present_family_queue_index,
			0,
			&_present_queue 
		);	
	}
	Device::~Device() {
		vkDestroyDevice( _device, nullptr );
	}
}
