#include "PhysicalDevice.hpp"
#include <cstdint>
#include <iostream>

namespace gfx
{
	PhysicalDevice::PhysicalDevice( Instance& vulkan_instance ) :
		_physical_device{}
	{
		
		uint32_t physical_device_count = 0;
		vkEnumeratePhysicalDevices(
			vulkan_instance.get(), 
			&physical_device_count,
			nullptr
		);
		auto physical_devices = std::vector<VkPhysicalDevice>{ physical_device_count };
		vkEnumeratePhysicalDevices(
			vulkan_instance.get(), 
			&physical_device_count,
			physical_devices.data()
		);

		for( auto physical_device : physical_devices ) {
	  		auto properties = VkPhysicalDeviceProperties();
	  		auto features = VkPhysicalDeviceFeatures();
			vkGetPhysicalDeviceProperties( physical_device, &properties );
			vkGetPhysicalDeviceFeatures( physical_device, &features );

			if( properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ) {
				_physical_device = VkPhysicalDevice( physical_device ); 
			}
		}

		if( this->get() == nullptr ) 
			std::cout << "No Integrated GPU available \n";
	}

	PhysicalDevice::~PhysicalDevice(){}
}
