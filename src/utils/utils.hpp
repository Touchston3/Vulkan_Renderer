#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

namespace Utils {

	inline std::vector<char> load_shader( const std::string& file_path ) {

		std::ifstream file_stream = std::ifstream( file_path, std::ios::binary | std::ios::ate );

		if( !file_stream.is_open() )
			std::cout << "Cannot open file \n";

		size_t file_size = file_stream.tellg();

		std::vector<char> file_data( file_size );

		file_stream.seekg(0);
		file_stream.read( file_data.data(), file_size );
		file_stream.close();

		return file_data;
	}
}
