#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>

class Window 
{
	struct GLFWwindow_deleter
	{
		void operator()(GLFWwindow* window)
		{ 
			glfwDestroyWindow(window);
		};
	};
public:
	Window(uint32_t width, uint32_t height, const std::string& name);
	~Window();

	bool should_close();

	uint32_t _width;
	uint32_t _height;
	std::string _name;
	std::unique_ptr<GLFWwindow, GLFWwindow_deleter> _handle;
	std::vector<const char*> _extensions;

};
