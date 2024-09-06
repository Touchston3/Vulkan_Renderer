#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN
#define VK_VERSION_1_0

#include "Window.hpp"
#include <iostream>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>

using namespace std;
Window::Window(uint32_t width, uint32_t height, const string& name)
{
	_width = width;
	_height = height;
	_name = name;
	if( !glfwInit() )
		std::cout << "Error in GLFW Init \n";

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_handle = (unique_ptr<GLFWwindow, GLFWwindow_deleter>) glfwCreateWindow
	(
		_width, 
		_height,
		_name.c_str(),
		nullptr, 
		nullptr
  	);

	uint32_t extension_count = 0;
	auto extensions_raw = (const char**) glfwGetRequiredInstanceExtensions(&extension_count);

	_extensions = vector<const char*> ( extensions_raw, extensions_raw + extension_count );
	_extensions.push_back("VK_EXT_debug_utils");
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::should_close(){ return glfwWindowShouldClose(_handle.get()); }
