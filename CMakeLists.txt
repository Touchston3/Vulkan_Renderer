#This whole thing is cursed. Gonna redo it at somepoint so I don't need to manually link the binaries.
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES 0)
##set(CMAKE_TOOLCHAIN_FILE CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake) saving this for when I finally figure out glfw linking

project(vulkan_renderer)
set(VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp )
file(GLOB_RECURSE header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )


find_package(Vulkan REQUIRED)
#find_package(glfw3 REQUIRED) 
#find_package(Backward  REQUIRED )

add_executable(Renderer ${source_files} ${header_files})


target_link_libraries(Renderer PRIVATE 
	${Vulkan_LIBRARY}
	${VENDOR_PATH}/lib/libglfw3.a ##WHY IS GLFW SUCH A PROBLEM CHILD?
	${VENDOR_PATH}/lib/libbackward.a
	
)
