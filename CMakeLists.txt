cmake_minimum_required(VERSION 3.29.2)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES 0)

project(vulkan_renderer)

set(glfw_include_path ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw3/include)
file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${glfw_include_path}/*)

find_package(Vulkan REQUIRED)
#find_package(glfw3 REQUIRED) -- This just isn't working with vcpkg or conan on my linux mint machine. Works great if I install via apt tho 

add_executable(Renderer ${source_files} ${header_files})
target_include_directories(Renderer PUBLIC vendor/glfw3/include)
target_link_libraries(Renderer ${Vulkan_LIBRARIES} ${PROJECT_SOURCE_DIR}/vendor/glfw3/lib/libglfw3.a)
