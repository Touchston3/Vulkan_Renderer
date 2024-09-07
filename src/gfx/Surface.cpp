#include "Surface.hpp"
#include <GLFW/glfw3.h>

using namespace std;
namespace gfx
{
	Surface::Surface( Instance* vulkan_instance, Window& window )
	{
		_vulkan_instance = vulkan_instance;
		auto result = (VkResult) glfwCreateWindowSurface
		( 
			vulkan_instance->get(),
			window.get(),
			nullptr,
			&_surface
		);
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(_vulkan_instance->get(), this->get(), nullptr);
	}
}
