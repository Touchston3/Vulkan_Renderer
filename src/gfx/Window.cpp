
#include "Window.hpp"
#include <iostream>
#include "../utils/Exception.hpp"

using namespace std;

namespace gfx {

	Window::Window(uint32_t width, uint32_t height, const string& name) :
		_width{ width },
		_height{ height },
		_name{ name },
		_extensions{},
		_window{}
	{
		if( !glfwInit() )
			throw Utils::Exception<int>{ "GLFW Init Error" };

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		if( glfwPlatformSupported(GLFW_PLATFORM_WAYLAND) ) //So I just learned that Hyprland will not show a window if there is no framebuffer. So no good sanity check for this anymore XD
			glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

		_window = (unique_ptr<GLFWwindow, GLFWwindow_deleter>) glfwCreateWindow (
			_width, 
			_height,
			_name.c_str(),
			nullptr, 
			nullptr 
		);
		if( _window == nullptr )
			throw Utils::Exception<int>( "Window Create Failed" );

		uint32_t extension_count = 0;
		auto extensions_raw = (const char**) glfwGetRequiredInstanceExtensions(&extension_count);

		_extensions = vector<const char*> ( extensions_raw, extensions_raw + extension_count );
		_extensions.push_back("VK_EXT_debug_utils");
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::should_close(){ return glfwWindowShouldClose( this->get() ); }
}
