package main

import "core:fmt"
import "vendor:vulkan"
import "vendor:glfw"
import "vendor:glfw/bindings"

main :: proc()
{
	fmt.printf("Hello World \n")

	glfw.Init()
	defer glfw.Terminate()

	window: glfw.WindowHandle = glfw.CreateWindow(640, 480, "Test Window", nil, nil)
	defer glfw.DestroyWindow(window)

	glfw_extension_count: u32 = 0
	glfw_extension_names: [^]cstring = bindings.GetRequiredInstanceExtensions(&glfw_extension_count)

	
	vulkan.load_proc_addresses((rawptr)(glfw.GetInstanceProcAddress))

	app_info: vulkan.ApplicationInfo = 
	{
		sType = vulkan.StructureType.APPLICATION_INFO,
		pApplicationName = "Vulkan Renderer",
		applicationVersion = vulkan.MAKE_VERSION(1, 0, 0),
		pEngineName = "Testing",
		engineVersion = vulkan.MAKE_VERSION(1, 0, 0),
		apiVersion = vulkan.API_VERSION_1_0,
	}

	ENABLED_LAYER_COUNT :: 0
	create_info: vulkan.InstanceCreateInfo = 
	{
		sType = vulkan.StructureType.INSTANCE_CREATE_INFO,
		pApplicationInfo = &app_info,
		enabledExtensionCount = glfw_extension_count,
		ppEnabledExtensionNames = glfw_extension_names,
		enabledLayerCount = ENABLED_LAYER_COUNT,
	}

	vulkan_instance: vulkan.Instance = {}

	if vulkan.CreateInstance(&create_info, nil, &vulkan_instance) != vulkan.Result.SUCCESS 
	{
		fmt.printf("Issue Creating Vulkan Instance \n")
	}


	vk_extension_count: u32 = 0
	vulkan.EnumerateInstanceExtensionProperties(nil, &vk_extension_count, nil)


	for !glfw.WindowShouldClose(window) 
	{
		glfw.PollEvents()
	}
	
}
