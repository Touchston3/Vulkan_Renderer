cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)

project(vulkan_renderer)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*")

add_executable(${PROJECT_NAME} ${sources})
target_include_directories(vulkan_renderer PUBLIC ${Vulkan_INCLUDE_DIRS} ${GLFW_INLCUDE_DIRS})
target_link_directories(vulkan_renderer PUBLIC ${Vulkan_LIBRARIES} ${GLFW_LIB})
target_link_libraries(vulkan_renderer PUBLIC ${Vulkan_LIBRARIES} glfw)
