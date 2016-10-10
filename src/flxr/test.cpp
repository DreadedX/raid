//----------------------------------------------
#include <iostream>

#include "flxr/write.h"
#include "flxr/read.h"

using namespace flxr;
//----------------------------------------------
void write_test() {
	std::cout << "WRITE TEST\n";

	Container container;
	container.name = "test.flx";

	container.add_file(File("test1"));
	container.add_file(File("test2"));

	container.configure(COMPRESSION::ZLIB);

	std::fstream stream;
	stream.open(container.name, std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);

	if(!stream.is_open()) {
		std::cout << "Failed to open file: " << container.name << '\n';
		exit(-1);
	}

	write_header(stream, container);
	write_index(stream, container);
	write_data(stream, container);
	write_crc(stream);

	stream.close();
}
//----------------------------------------------
void read_test() {
	std::cout << "READ TEST\n";

	Container read_container;
	read_container.name = "test.flx";
	std::fstream stream;
	stream.open(read_container.name, std::ios::in | std::ios::binary);

	check_crc(stream);
	read_header(stream, read_container);
	read_index(stream, read_container);

	stream.close();
}
//----------------------------------------------
int main() {
	write_test();
	read_test();
}
