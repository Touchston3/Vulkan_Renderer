#pragma once
#include <string>

namespace Utils
{
	template<typename DATA_TYPE>
	class Exception
	{
		public:
			Exception(const std::string message)
			{
				_message = message;
			}


			DATA_TYPE _data;
			std::string _message;
	};
}

