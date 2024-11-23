#include "SwapChain.hpp"
#include "../utils/Exception.hpp"
#include <algorithm>
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace gfx {

	SwapChain::SwapChain(Device* device, PhysicalDevice& physical_device, Surface& surface, Window& window) :
		_images{},
		_image_views{},
		_image_extent{},
		_device{ device },
		_swap_chain{}
	{
		uint32_t surface_format_cout = 0;
		uint32_t present_mode_count = 0;

		vkGetPhysicalDeviceSurfaceFormatsKHR( 
			physical_device.get(),
			surface.get(),
			&surface_format_cout,
			nullptr 
		);
		vkGetPhysicalDeviceSurfacePresentModesKHR( 
			physical_device.get(),
			surface.get(),
			&present_mode_count, 
			nullptr 
		);

		auto surface_formats = std::vector<VkSurfaceFormatKHR>{ surface_format_cout };
		auto surface_present_modes = std::vector<VkPresentModeKHR>{ present_mode_count };

		vkGetPhysicalDeviceSurfaceFormatsKHR(
			physical_device.get(),
			surface.get(),
			&surface_format_cout,
			surface_formats.data() 
		);
		vkGetPhysicalDeviceSurfacePresentModesKHR( 
			physical_device.get(),
			surface.get(),
			&present_mode_count,
			surface_present_modes.data() 
		);


		//Choose swapchain format
		for( auto surface_format : surface_formats )
		{
			if( surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
				_format = surface_format;
		}

		//Choose present mode
		auto active_surface_present_mode = VK_PRESENT_MODE_FIFO_KHR;
		for( auto surface_present_mode : surface_present_modes )
		{
			if( surface_present_mode == VK_PRESENT_MODE_MAILBOX_KHR )
				active_surface_present_mode = surface_present_mode;
		}

		//Set swapchain image resolution
		auto surface_capabilities = VkSurfaceCapabilitiesKHR{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physical_device.get(), surface.get(), &surface_capabilities );

		//Figure out image extent
		int width, height;
		glfwGetFramebufferSize( window.get(), &width, &height );
		_image_extent = VkExtent2D {
			std::clamp( static_cast<uint32_t>(width), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width ),
			std::clamp( static_cast<uint32_t>(height), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height )
		};
	
		auto swap_chain_create_info = VkSwapchainCreateInfoKHR {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = surface.get(),
			.minImageCount = surface_capabilities.minImageCount+1,
			.imageFormat = _format.format,
			.imageColorSpace = _format.colorSpace,
			.imageExtent = _image_extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE, // Based on if graphics family queue and present queue are the same
			.queueFamilyIndexCount = 0, // Same with this
			.pQueueFamilyIndices = nullptr, // And this
			.preTransform = surface_capabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = active_surface_present_mode,
			.clipped = VK_TRUE,
			.oldSwapchain = VK_NULL_HANDLE,
		};
		if( vkCreateSwapchainKHR(
			_device->get(), 
			&swap_chain_create_info, 
			nullptr,
			&_swap_chain 
		) != VkResult::VK_SUCCESS ){
			throw Utils::Exception<int>{};
		}

		//Create Images and Image Views
		{
			uint32_t image_count = 0;

			vkGetSwapchainImagesKHR(
				_device->get(), 
				_swap_chain,
				&image_count,
				nullptr 
			);
			_images.resize(image_count);
			_image_views.resize(image_count);

			vkGetSwapchainImagesKHR( 
				_device->get(), 
				_swap_chain,
				&image_count,
				_images.data() 
			);
		
			for( size_t i = 0; i < image_count; i++ ) {
				auto image_view_info = VkImageViewCreateInfo {
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.flags = {},
					.image = _images[i],
					.viewType = VK_IMAGE_VIEW_TYPE_2D,
					.format = _format.format,
					.components = {
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY,
					},
					.subresourceRange = {
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1,
					},
				};

				if( vkCreateImageView(
					_device->get(),
					&image_view_info,
					nullptr,
					&_image_views[i] 
				) != VkResult::VK_SUCCESS ) {
					throw Utils::Exception<int>{};
				}
			}
		}
	}


	SwapChain::~SwapChain() {
		for( auto image_view : _image_views )
			vkDestroyImageView(_device->get(), image_view, nullptr);	

		vkDestroySwapchainKHR(_device->get(), _swap_chain, nullptr);
	}
}
