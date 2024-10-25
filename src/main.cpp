#include "app.hpp"
#include <iostream>
#include "utils/Exception.hpp"

int main()
{
	try {
		auto app = App{};
		app.init();
		app.run();
		app.cleanup();
	}
	catch( Utils::Exception<int> e ) {
		e.print();	
	}
	catch( std::exception e ) {
		std::cout << e.what() << "\n";
	}
	catch( ... ) { 
		std::cout << "Lmao bro wtf is this? \n";
	}
}
