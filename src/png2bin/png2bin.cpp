//----------------------------------------------
#include <iostream>
#include "flxr/binary_helper.h"
//----------------------------------------------
extern "C" {
	std::iostream& process(std::iostream& stream) {
		std::cout << "Hello world, from png2bin\n";

		// Test to see if we can actually load the file from stream
		int test = 0;
		flxr::read(stream, test);
		stream.seekg(0, std::ios::beg);

		std::cout << "[D] " << test << '\n';

		/// @todo: I am pretty sure the reference will go out of scope
		return stream;
	}
}
//----------------------------------------------
