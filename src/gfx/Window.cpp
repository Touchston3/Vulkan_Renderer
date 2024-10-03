
#include "Window.hpp"
#include <iostream>
#include "../utils/Exception.hpp"

using namespace std;
namespace gfx
{
Window::Window(uint32_t width, uint32_t height, const string& name) :
	_width(0),
	_height(0),
	_name(""),
	_extensions(vector<const char*>()),
	_window(nullptr)
{
	_width = width;
	_height = height;
	_name = name;
	if( !glfwInit() )
		throw Utils::Exception<int>{ "GLFW Init Error" };

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_window = (unique_ptr<GLFWwindow, GLFWwindow_deleter>) glfwCreateWindow
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

bool Window::should_close(){ return glfwWindowShouldClose( this->get() ); }
}
