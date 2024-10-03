#pragma once
#include "../../vendor/glfw3/include/GLFW/glfw3.h" //I need to fix this
#include <cstdint>
#include <vector>
#include <memory>
#include <string>

namespace gfx
{
	class Window 
	{
		private:
			struct GLFWwindow_deleter
			{
				void operator()(GLFWwindow* window)	{ glfwDestroyWindow(window); };
			};
		public:
			Window(uint32_t width, uint32_t height, const std::string& name);
			~Window();
			bool should_close();
			inline GLFWwindow* get() { return _window.get(); }

			uint32_t _width;
			uint32_t _height;
			std::string _name;
			std::vector<const char*> _extensions;

		private:
			std::unique_ptr<GLFWwindow, GLFWwindow_deleter> _window;
	};
}
