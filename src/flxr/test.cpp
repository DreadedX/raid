//----------------------------------------------
#include <iostream>

#include "shared/logger.h"

#include "flxr/write.h"
#include "flxr/read.h"

using namespace flxr;
//----------------------------------------------
/// @todo This is terrible, for the actual tool this needs some work
namespace Progress {
	std::string name;
	uint64 total_size;

	void draw(uint64 total_read) {

		float progress = float(total_read)/float(total_size);

		int bar_width = 70;
		std::cout << name << "\t[";
		int pos = bar_width * progress;
		for (int i = 0; i < bar_width; ++i) {
			if (i < pos) {
				std::cout << "=";
			} else if (i == pos) {
				std::cout << ">";
			} else {
				std::cout << " ";
			}
		}
		std::cout << "] " << int(progress * 100) << "%\r";
		std::cout.flush();
	}

	void setup(const std::string& m_name, uint64 m_total_size) {
		name = m_name;
		total_size = m_total_size;

		draw(0);
	}

	void finish() {
		std::cout << '\n';
	}
}
//----------------------------------------------
void write_test() {
	debug << "WRITE TEST\n";

	Container container;
	container.name = "test.flx";

	container.add_file(File("test.txt"));
	container.add_file(File("sponza.obj"));

	container.configure(COMPRESSION::ZLIB);

	std::fstream stream;
	stream.open(container.name, std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);

	if(!stream.is_open()) {
		error << "Failed to open file: " << container.name << '\n';
		exit(-1);
	}

	write_header(stream, container);
	write_index(stream, container);
	write_data(stream, container, 9, Progress::setup, Progress::draw, Progress::finish);
	write_index(stream, container);
	write_crc(stream);

	stream.close();
}
//----------------------------------------------
void read_test() {
	debug << "READ TEST\n";

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
