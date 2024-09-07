#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>


namespace gfx
{
class Device
{
public:
	Device(vk::PhysicalDevice physical_device, const std::vector<const char*>& validation_layers);
	~Device();

	inline VkDevice& get() { return _device; }

	VkQueue _queue;
private:
	VkDevice _device;
};
}
