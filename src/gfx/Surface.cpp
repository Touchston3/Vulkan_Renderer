#include "Surface.hpp"

using namespace std;

namespace gfx {

	Surface::Surface( Instance* vulkan_instance, Window& window ) :
		_surface{},
		_vulkan_instance{ vulkan_instance }
	{
		glfwCreateWindowSurface( 
			vulkan_instance->get(),
			window.get(),
			nullptr,
			&_surface
		);
	}

	Surface::~Surface() {
		vkDestroySurfaceKHR( _vulkan_instance->get(), this->get(), nullptr );
	}
}
