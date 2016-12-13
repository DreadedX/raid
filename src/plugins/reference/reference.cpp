//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/memstream.h"
#include "flexy/helper.h"
#include "flexy/plugin.h"

#include "logger.h"
//----------------------------------------------
class Reference : public Plugin {
	public:
		virtual std::shared_ptr<std::iostream> process(std::string file_path) override {

			debug << "Hello world, from png2bin\n";
			debug << file_path << '\n';

			// Prints the first four bytes in hex as a demonstration
			auto stream = open_file(file_path);
			byte magic[4];
			flxr::read(*stream, magic);
			stream->seekg(0, std::ios::beg);
			debug << std::hex << (int)magic[0] << (int)magic[1] << (int)magic[2] << (int)magic[3] << '\n';

			// Demonstates how to return an array
			byte buffer[] = {72, 101, 108, 108, 111};
			std::shared_ptr<std::iostream> final_stream = std::make_shared<Memstream<char*>>((char*)buffer, sizeof(buffer));

			// Demonstates how to return a string
			// std::string data = "This is an example text";
			// std::shared_ptr<std::iostream> final_stream = std::make_shared<Memstream<std::string>>(data);

			return final_stream;
		}
};
//----------------------------------------------
share_plugin(Reference);
//----------------------------------------------

