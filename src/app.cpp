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
#include "gfx/Window.hpp"
#include "gfx/Instance.hpp"
#include "gfx/PhysicalDevice.hpp"
#include "gfx/Surface.hpp"
#include "gfx/Device.hpp"

App::App(){}
App::~App(){}

void App::init(){}

void App::run()
{
	auto window = gfx::Window(640, 480, "Test Window");
	//Create Validation Layers -- Need to do actual debugging
	auto vk_validation_layers = std::vector<const char*>();
	auto device_extensions = std::vector<const char*>();
	{
		vk_validation_layers.push_back("VK_LAYER_KHRONOS_validation");
		device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		uint32_t vk_extension_count = 0;
		uint32_t vk_layer_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &vk_extension_count, nullptr);
		vkEnumerateInstanceLayerProperties(&vk_layer_count, nullptr);

		auto vk_extensions = std::vector<VkExtensionProperties>(vk_extension_count);
		auto vk_layer_properties = std::vector<VkLayerProperties>(vk_layer_count);

		vkEnumerateInstanceExtensionProperties(nullptr, &vk_extension_count, vk_extensions.data());
		vkEnumerateInstanceLayerProperties(&vk_layer_count, vk_layer_properties.data());

		// TODO: Enumerate extensions and layers and check if they include the things we need. 
	}
		
	//Create Instance
	auto vulkan_instance = gfx::Instance(window, vk_validation_layers);

	//Create Physical Device
	auto physical_device = gfx::PhysicalDevice(vulkan_instance);

	//Create Logical Device
	auto device = gfx::Device(physical_device, vk_validation_layers, device_extensions);
	
	//Create Surface
	auto surface = gfx::Surface(&vulkan_instance, window);
	{
		uint32_t surface_format_cout = 0;
		uint32_t present_mode_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device.get(), surface.get(), &surface_format_cout, nullptr);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device.get(), surface.get(), &present_mode_count, nullptr);
		auto surface_formats = std::vector<VkSurfaceFormatKHR>(surface_format_cout);
		auto surface_present_modes = std::vector<VkPresentModeKHR>(present_mode_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device.get(), surface.get(), &surface_format_cout, surface_formats.data());
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device.get(), surface.get(), &present_mode_count, surface_present_modes.data());


		//Choose swapchain format

		auto active_format = VkSurfaceFormatKHR();
		for( auto surface_format : surface_formats )
		{
			if( surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
				active_format = surface_format;
		}

		//Choose present mode
		auto active_surface_present_mode = VK_PRESENT_MODE_FIFO_KHR;
		for( auto surface_present_mode : surface_present_modes )
		{
			if( surface_present_mode == VK_PRESENT_MODE_MAILBOX_KHR )
				active_surface_present_mode = surface_present_mode;
		}

		//Set swapchain image resolution
		int pixel_width, pixel_height = 0;
		glfwGetFramebufferSize(window.get(), &pixel_width, &pixel_height);

	}

	auto vert_shader_binaries = (std::vector<char>) utils::load_shader("../../resources/shaders/bin/test.vert.spv");
	auto frag_shader_binaries = (std::vector<char>) utils::load_shader("../../resources/shaders/bin/test.frag.spv");
	



	while( !window.should_close() )
	{
		glfwPollEvents();
	}


}

void App::cleanup(){}
