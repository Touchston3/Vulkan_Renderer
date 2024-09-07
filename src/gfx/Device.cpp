#include "Device.hpp"


#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>

using namespace std;
namespace gfx
{
Device::Device(vk::PhysicalDevice physical_device, const vector<const char*>& validation_layers)
{
	bool has_graphics_queue_support = false;
	uint32_t graphics_family_queue = 0;

	auto queue_family_properties = (vector<vk::QueueFamilyProperties>) physical_device.getQueueFamilyProperties();

	int i = 0;
	for( auto queue_family : queue_family_properties )
	{
		if( queue_family.queueFlags & vk::QueueFlagBits::eGraphics )
		{	
			has_graphics_queue_support = true;
			graphics_family_queue = i;
		}
		i++;
	}

	//setup logical device and graphics queue
	float queue_priority = 1.f;
	uint32_t queue_create_info_count = 1;
	auto queue_create_info = vk::DeviceQueueCreateInfo
		(
			vk::DeviceQueueCreateFlags(),
			graphics_family_queue,
			1,
			&queue_priority
		);

	auto device_features = vk::PhysicalDeviceFeatures();
	auto device_info = vk::DeviceCreateInfo
		(
			vk::DeviceCreateFlags(),
			queue_create_info_count,
			&queue_create_info,
			static_cast<uint32_t>(validation_layers.size()),
			validation_layers.data(),
			0, nullptr,
			&device_features
		);

	try {
		_device = physical_device.createDevice(device_info);
		vkGetDeviceQueue(this->get(), graphics_family_queue, 0, &_queue);	
	} catch( vk::SystemError e ) {
		cout << e.what() << "\n";
	}
}
Device::~Device()
{
	vkDestroyDevice(_device,nullptr);
}
}
