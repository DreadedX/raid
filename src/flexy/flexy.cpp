//----------------------------------------------
#include <iostream>
#include <iomanip>

#include "flexy/ValTree.h"

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
COMPRESSION get_compression_type(ValTree package) {
	const std::string compression = package.query("compression.type").getStr();

	if (compression.compare("ZLIB") == 0) {
		return COMPRESSION::ZLIB;
	} else if (compression.compare("RAW") == 0) {
		return COMPRESSION::RAW;
	} else if (compression.compare("ON_DISK") == 0) {
		return COMPRESSION::ON_DISK;
	} else {
		std::cerr << "Invalid compression type\n";
		exit(-1);
	}
}
//----------------------------------------------
void write_test() {

	/// @todo The path to the config needs to be an commandline argument, and everything needs to be relative to the config file
	std::string base_path = "../../";

	ValTree v;
	v.parse(base_path + "assets/config.flxr");

	for (const auto& package : v.getChild("packages")) {

		std::cout << "[D] " << "WRITE TEST\n";

		Container container(package.getKey() + ".flx");

		for (auto file : package.getChild("files")) {
			container.add_file( MetaData(package.getKey() + "/" + file.getKey()) );
		}

		container.configure(get_compression_type(package), package.query("compression.level").getInt());
		std::cout << "[D] " << "Compression type: " << get_compression_type(package) << ", level: " << package.query("compression.level").getInt() << '\n';

		container.clear_file();
		write_header(container);
		write_index(container);
		for (auto& meta_data : container.get_index()) {
			std::fstream stream;
			std::string file_path = base_path + package.getChild("path").getStr() + "/" + meta_data.get_name().substr(meta_data.get_name().find_first_of('/')+1);
			meta_data.set_path(file_path);
			stream.open(file_path, std::ios::out | std::ios::in | std::ios::binary);

			std::cout << "[D] " << file_path << " -> " << meta_data.get_name() << '\n';

			// Find the file extension
			std::string extension = "";
			if (meta_data.get_name().substr(meta_data.get_name().find_last_of('/')).find_last_of('.') != std::string::npos) {
				extension = meta_data.get_name().substr(meta_data.get_name().find_last_of('.'));
			}
			std::cout << "[D] " << "Using plugin: " << v.query("plugins" + extension).getStr() << '\n';

			if (!stream.is_open()) {
				std::cerr << "Failed to open: " << file_path << '\n';
				exit(-1);
			}

			write_data(container, meta_data, stream, Progress::setup, Progress::draw, Progress::finish);

			stream.close();
		}
		write_index(container);
		write_crc(container);

		std::cout << "==============================\n";
	}
}
//----------------------------------------------
void read_test() {
	/// @todo The path to the config needs to be an commandline argument, and everything needs to be relative to the config file
	std::string base_path = "../../";

	ValTree v;
	v.parse(base_path + "assets/config.flxr");

	for (const auto& package : v.getChild("packages")) {

		std::cout << "[D] " << "READ TEST\n";

		Container container(package.getKey() + ".flx");

		check_crc(container);
		read_header(container);
		read_index(container);

		for (auto& meta_data : container.get_index()) {
			std::cout << meta_data.get_name() << " " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(meta_data.get_size())/1000/1000 << " MB compressed\n";
		}

		for (auto& meta_data : container.get_index()) {
			std::stringstream stream;
			read_data(container, meta_data, stream);
		}

		std::cout << "==============================\n";
	}
}
//----------------------------------------------
int main() {
	write_test();
	read_test();
}