#include "Device.hpp"
#include "iostream"

namespace gfx {
	Device::Extensions::Extensions( const std::vector<const char*>& required_extensions ) :
		_extensions{ required_extensions } 
	{ 
		_extensions.resize( _extensions.size() ); //Is this necessary?
	
		for( auto extension : _extensions )
			std::cout << extension << "\n";

		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );

		auto extension_properties = std::vector<VkExtensionProperties>{ extension_count };
		vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, extension_properties.data() );

		//Iterate Extensions and check
	}
}

