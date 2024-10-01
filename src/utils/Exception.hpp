#include <string>
#include <stacktrace>

namespace Utils
{
	template<typename DATA_TYPE>
	class Exception
	{
		public:
			Exception();
			Exception(DATA_TYPE data) :
				_message{ "" }
			{}
			Exception(const std::string& message, DATA_TYPE data) : 
				_data{ data },
				_message{ message },
				_stacktrace{ std::stacktrace::current() },
				_location{}
			{}

			~Exception() = default;
		 
			DATA_TYPE _data;
			std::string _message;
			std::stacktrace _stacktrace;
			std::source_location _location;
			
	};
}

