//----------------------------------------------
#include <iostream>
#include <iomanip>

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
		std::cout << "] " << int(progress * 100) << "%\t " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(total_read)/1000/1000 << " MB / " << float(total_size)/1000/1000 << " MB\r";
		std::cout.flush();
	}

	void setup(const std::string& m_name, uint64 m_total_size) {
		name = m_name;
		total_size = m_total_size;

		draw(0);
	}

	void finish(uint64 compressed_size) {
		std::cout << '\n';
		std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(compressed_size)/1000/1000 << " MB compressed (" << float(compressed_size)/float(total_size)*100 << "%)\n";
	}
}
//----------------------------------------------
void write_test() {
	std::cout << "[D] " << "WRITE TEST\n";

	Container container("test.flx");

	container.add_file(File("test.txt"));
	container.add_file(File("sponza.obj"));

	container.configure(COMPRESSION::ZLIB, 9);

	container.clear_file();
	write_header(container);
	write_index(container);
	write_data(container, Progress::setup, Progress::draw, Progress::finish);
	write_index(container);
	write_crc(container);
}
//----------------------------------------------
void read_test() {
	std::cout << "[D] " << "READ TEST\n";

	Container read_container("test.flx");

	check_crc(read_container);
	read_header(read_container);
	read_index(read_container);
}
//----------------------------------------------
int main() {
	write_test();
	read_test();
}
