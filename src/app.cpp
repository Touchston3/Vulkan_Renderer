#include "gfx/Pipeline.hpp"
#define GLFW_INCLUDE_VULKAN
//#define VK_VERSION_1_0

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vector>
#include <cstdint>

#include "app.hpp"
#include "gfx/Window.hpp"
#include "gfx/Instance.hpp"
#include "gfx/PhysicalDevice.hpp"
#include "gfx/Surface.hpp"
#include "gfx/Device.hpp"
#include "gfx/SwapChain.hpp"
#include "utils/Exception.hpp"

App::App(){}
App::~App(){}

void App::init(){}

void App::run() {

	auto window = gfx::Window{ 640, 480, "Test Window" };

	//Create Validation Layers -- Need to do actual debugging
	auto vk_validation_layers = std::vector<const char*>{};
	{
		vk_validation_layers.push_back("VK_LAYER_KHRONOS_validation");

		uint32_t vk_layer_count = 0;
		vkEnumerateInstanceLayerProperties(&vk_layer_count, nullptr);

		auto vk_layer_properties = std::vector<VkLayerProperties>(vk_layer_count);

		vkEnumerateInstanceLayerProperties(&vk_layer_count, vk_layer_properties.data());

		// TODO: Enumerate extensions and layers and check if they include the things we need. 
	}
		
	//Create Instance
	auto vulkan_instance = gfx::Instance{ window, vk_validation_layers };

	//Create Physical Device
	auto physical_device = gfx::PhysicalDevice{ vulkan_instance };

	//I need to understand how extensions work better and refactor this so it makes more sense.
	auto device_extensions= gfx::Device::Extensions {
		std::vector<const char*> {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		}
	};
	auto device = gfx::Device {
		physical_device,
		vk_validation_layers,
		device_extensions
	};

	//Create Surface
	auto surface = gfx::Surface{ &vulkan_instance, window };

	//swap chain
	auto swap_chain = gfx::SwapChain{ &device, physical_device, surface, window };
	
	//pipeline
	auto pipeline = gfx::Pipeline{ device, swap_chain };

	//Shaders

	while( !window.should_close() ) {
		glfwPollEvents();
	}


}

void App::cleanup(){}
