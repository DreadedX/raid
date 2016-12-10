//----------------------------------------------
#include <iostream>
#include <memory>
#include "flxr/binary_helper.h"
#include "flxr/memstream.h"
#include "typedef.h"
//----------------------------------------------
extern "C" {
	std::shared_ptr<std::iostream> process(std::shared_ptr<std::iostream> stream) {
		std::cout << "Hello world, from png2bin\n";

		// Test to see if we can actually load the file from stream
		int test = 0;
		flxr::read(*stream, test);
		stream->seekg(0, std::ios::beg);

		std::cout << "[D] " << test << '\n';

		byte buffer[] = "This is something really awesome!";
		std::shared_ptr<std::iostream> final_stream = std::make_shared<Memstream<char*>>((char*)buffer, sizeof(buffer));

		// std::string data = "Shoutcraft Kings 2016!";
		// std::shared_ptr<std::iostream> final_stream = std::make_shared<Memstream<std::string>>(data);

		/// @note Somehow this break everything...
		// std::string line;
		// while (std::getline(*final_stream, line, ' ')) {
		// 	std::cout << "line: " << line << '\n';
		// }
		
		return final_stream;
	}
}
//----------------------------------------------
