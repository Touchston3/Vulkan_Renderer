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

App::App(){}
App::~App(){}

void App::init(){}

void App::run()
{
	// -- GLFW INIT
	GLFWwindow* window(nullptr);
	auto glfw_extensions = std::vector<const char*>();
 	{
		const uint32_t WIDTH = 640;
		const uint32_t HEIGHT= 480;

		if( !glfwInit() )
			std::cout << "Error in GLFW Init \n";

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(WIDTH, HEIGHT,"Vulkan App", nullptr, nullptr);

		uint32_t glfw_extension_count = 0;
		auto glfw_extension_names = (const char**) glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		glfw_extensions = std::vector<const char*>(glfw_extension_names, glfw_extension_names + glfw_extension_count);

		//Add debug utils
		glfw_extensions.push_back("VK_EXT_debug_utils");
	}

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
			static_cast<uint32_t>(glfw_extensions.size()), //Extensions
			glfw_extensions.data(), //Extensions
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

	  		std::cout << properties.deviceName << "\n";
			if( properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU )
			{
				vk_device = vk::PhysicalDevice(device); 
			}
		}

		if( vk_device == nullptr ) 
			std::cout << "No Integrated GPU available \n";
	}




	//Create Surface
	auto vk_surface = vk::SurfaceKHR();
	{
		auto result = (vk::Result) glfwCreateWindowSurface
		( 
			static_cast<VkInstance>(vk_instance),
			window,
			nullptr,
			reinterpret_cast<VkSurfaceKHR*>(&vk_surface)
		);
		
		//TODO: Actual Debugging
		std::string result_string;
		try {
			vk::resultCheck(result, result_string.c_str());
		} catch (vk::SystemError e)
		{
			std::cout << "issue creating vulkan-glfw surface " << e.what() << "\n";
		}
	}

	
	auto vert_shader_binaries = (std::vector<char>) utils::load_shader("../../resources/shaders/bin/test.vert.spv");
	auto frag_shader_binaries = (std::vector<char>) utils::load_shader("../../resources/shaders/bin/test.frag.spv");
	



	while( !glfwWindowShouldClose(window) )
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
	vkDestroyInstance(vk_instance, nullptr);
}

void App::cleanup(){}
