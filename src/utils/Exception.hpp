#pragma once
#include <string>

namespace Utils
{
	template<typename DATA_TYPE>
	class Exception : std::exception
	{
		public:
			Exception() :
				_data{},
				_message{""}
			{}
			Exception(const std::string& message) :
				_data{},
				_message{ message }
			{}
			Exception(DATA_TYPE data, const std::string& message) :
				_data{ data },
				_message{ message }
			{
			}
			DATA_TYPE _data;
			std::string _message;
	};
}

