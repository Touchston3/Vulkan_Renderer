#pragma once
#include <execinfo.h>
#include <string>
#include "iostream"
#include "../../vendor/include/backward.hpp"

namespace Utils
{
	template<typename DATA_TYPE>
	class Exception : std::exception
	{
		public:
			Exception() :
				_data{},
				_message{""},
				_trace{}
			{ 
				_trace.load_here();
			}

			Exception(const std::string& message) :
				_data{},
				_message{ message },
				_trace{}
			{
				_trace.load_here();
			}
			Exception(DATA_TYPE data, const std::string& message) :
				_data{ data },
				_message{ message },
				_trace{}
			{
				_trace.load_here();
			}


			void print()
			{
				auto resolver = backward::TraceResolver{};
				resolver.load_stacktrace( _trace );
				
				std::cout << "Message: " << _message << "\n";
				std::cout << "Data: " << _data << "\n";
			
				for( size_t i = 0; i < _trace.size(); i++ ) {
					backward::ResolvedTrace resolved = resolver.resolve( _trace[i] );

					std::cout << "#" << i 
						<< " " << resolved.object_filename
						<< " " << resolved.object_function
						<< " [" << resolved.addr <<	"]"
						<< "\n";
				}
			}


			DATA_TYPE _data;
			std::string _message;
			backward::StackTrace _trace;
	};
}

