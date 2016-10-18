//----------------------------------------------
#include <iostream>
#include <iomanip>

#include "flxr/ValTree.h"

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

	Container container("../../assets/test.flx");

	container.add_file(File("../../assets/test/musketeers.txt"));

	container.configure(COMPRESSION::ZLIB, 9);

	container.clear_file();
	write_header(container);
	write_index(container);
	for(auto& file : container.get_files()) {
		std::fstream stream;
		stream.open(file.get_name(), std::ios::out | std::ios::in | std::ios::binary);

		if (!stream.is_open()) {
			std::cerr << "Failed to open: " << file.get_name() << '\n';
		}

		write_data(container, file, stream, Progress::setup, Progress::draw, Progress::finish);

		stream.close();
	}
	write_index(container);
	write_crc(container);
}
//----------------------------------------------
void read_test() {
	std::cout << "[D] " << "READ TEST\n";

	Container container("../../assets/test.flx");

	check_crc(container);
	read_header(container);
	read_index(container);

	for(auto& file : container.get_files()) {
		std::cout << file.get_name() << " " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(file.get_size())/1000/1000 << " MB compressed\n";
	}

	for(auto& file : container.get_files()) {
		std::stringstream stream;
		read_data(container, file, stream);
	}
}
//----------------------------------------------
int main() {
	write_test();
	read_test();

	// ValTree v;
	// v.parse("../../assets/config.flxr");
    //
	// auto& name = v.query("plugins.fbx.use");
    //
	// for (auto test : v.getChild("packages")) {
	// 	std::cout << test.getKey() << ": path=" << test.getChild("path").getStr() << '\n';
	// }
    //
	// if (name.isNull()) {
	// 	std::cout << "No plugin found for this file\n";
	// } else {
	// 	std::cout << name.getStr() << '\n';
	// }
}
