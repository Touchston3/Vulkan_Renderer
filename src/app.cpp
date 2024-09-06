#include <cstdint>
#define GLFW_INCLUDE_VULKAN
#define VK_VERSION_1_0

#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vector>

#include "app.hpp"
#include "utils.hpp"
#include "Window.hpp"

App::App(){}
App::~App(){}

void App::init(){}

void App::run()
{
	auto window = Window(640, 480, "Test Window");
	//Create Validation Layers -- Need to do actual debugging
	auto vk_validation_layers = std::vector<const char*>();
	{
		vk_validation_layers = std::vector<const char*>
		{
			"VK_LAYER_KHRONOS_validation",
		};
		uint32_t vk_extension_count = 0;
		uint32_t vk_layer_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &vk_extension_count, nullptr);
		vkEnumerateInstanceLayerProperties(&vk_layer_count, nullptr);

		auto vk_extensions = std::vector<VkExtensionProperties>(vk_extension_count);
		auto vk_layer_properties = std::vector<VkLayerProperties>(vk_layer_count);

		vkEnumerateInstanceExtensionProperties(nullptr, &vk_extension_count, vk_extensions.data());
		vkEnumerateInstanceLayerProperties(&vk_layer_count, vk_layer_properties.data());
	}
		


		// TODO: Enumerate extensions and layers and check if they include the things we need. 
		
	//Create Instance
	auto vk_instance = vk::Instance();
	{
		auto vk_app_info = vk::ApplicationInfo 
		{
			"Vulkan Renderer",
			VK_MAKE_VERSION(1, 0, 0),
			"Test Engine",
			VK_MAKE_VERSION(1, 0, 0),
			VK_API_VERSION_1_3,
			nullptr
		};

		auto vk_instance_info = vk::InstanceCreateInfo
		{
			vk::InstanceCreateFlags(),
			&vk_app_info,
			static_cast<uint32_t>(vk_validation_layers.size()), //Validation Layers
			vk_validation_layers.data(), //Validation Layers
			static_cast<uint32_t>(window._extensions.size()), //Extensions
			window._extensions.data(), //Extensions
		};

		if( vk::createInstance( &vk_instance_info, nullptr, &vk_instance ) != vk::Result::eSuccess)
			std::cout << "issue creating vulkan instance \n";
	}

	//Create Physical Device
	auto vk_device = vk::PhysicalDevice();
	{
		uint32_t physical_device_count = 0;
		vkEnumeratePhysicalDevices
		(
			static_cast<VkInstance>(vk_instance), 
			&physical_device_count,
			nullptr
		);
		auto devices = std::vector<VkPhysicalDevice>(physical_device_count);
		vkEnumeratePhysicalDevices
		(
			static_cast<VkInstance>(vk_instance), 
			&physical_device_count,
			devices.data()
		);

		for( auto device : devices )
		{
	  		auto properties = VkPhysicalDeviceProperties();
	  		auto features = VkPhysicalDeviceFeatures();
			vkGetPhysicalDeviceProperties(device, &properties);
			vkGetPhysicalDeviceFeatures(device, &features);

			if( properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
			{
				vk_device = vk::PhysicalDevice(device); 
			}
		}

		if( vk_device == nullptr ) 
			std::cout << "No Integrated GPU available \n";
	}

	//setup queue families
	bool has_graphics_queue_support = false;
	uint32_t graphics_family_queue = 0;
	{
		auto queue_family_properties = (std::vector<vk::QueueFamilyProperties>) vk_device.getQueueFamilyProperties();

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
	}

	//setup logical device and graphics queue
	auto logical_device = vk::Device();
	auto graphics_queue = vk::Queue();
	{
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
			static_cast<uint32_t>(vk_validation_layers.size()),
			vk_validation_layers.data(),
			0, nullptr,
			&device_features
		);

		try {
			logical_device = vk_device.createDevice(device_info);
		} catch( vk::SystemError e ) {
			std::cout << e.what() << "\n";
		}

		graphics_queue = logical_device.getQueue(graphics_family_queue, 0);
	}



	//Create Surface
	auto vk_surface = vk::SurfaceKHR();
	{
		auto result = (vk::Result) glfwCreateWindowSurface
		( 
			static_cast<VkInstance>(vk_instance),
			window._handle.get(),
			nullptr,
			reinterpret_cast<VkSurfaceKHR*>(&vk_surface)
		);
		//TODO: Actual Debugging
		try {
			std::string result_string;
			vk::resultCheck(result, result_string.c_str());
		} catch (vk::SystemError e) {
			std::cout << "issue creating vulkan-glfw surface " << e.what() << "\n";
		}
	}

	
	auto vert_shader_binaries = (std::vector<char>) utils::load_shader("../../resources/shaders/bin/test.vert.spv");
	auto frag_shader_binaries = (std::vector<char>) utils::load_shader("../../resources/shaders/bin/test.frag.spv");
	



	while( !window.should_close() )
	{
		glfwPollEvents();
	}

	vk_instance.destroySurfaceKHR(vk_surface);
	logical_device.destroy();
	vk_instance.destroy();

}

void App::cleanup(){}
