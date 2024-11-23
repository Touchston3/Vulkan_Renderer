#pragma once

#include <vector>
#include <string>
namespace Utils {

	class Files {

		Files() = delete;

		public:
			static std::vector<char> load_shader( const std::string& file_path );
	};
}
