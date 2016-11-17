//----------------------------------------------
#include <iostream>
#include <iomanip>
#include <dlfcn.h>

#include "flexy/ValTree.h"
#include "flexy/helper.h"

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
void process(std::string plugin_name, std::iostream& stream) {
	std::string base_path = "../bin/";
	plugin_name = base_path + "lib" + plugin_name + ".so";
	void* handle = dlopen(plugin_name.c_str(), RTLD_NOW);
	char* error = dlerror();
	if (error) {
		std::cerr << error << '\n';
		exit(-1);
	}
	delete[] error;
	error = nullptr;

	typedef void (*process_pointer)(std::iostream& stream);
	process_pointer process = (process_pointer)dlsym(handle, "process");
	error = dlerror();
	if (error) {
		std::cerr << error << '\n';
		exit(-1);
	}
	delete[] error;
	error = nullptr;

	process(stream);
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

		container.empty_file();
		write_header(container);
		write_index(container);
		for (auto& meta_data : container.get_index()) {
			std::fstream stream;
			std::string file_path = base_path + package.getChild("path").getStr() + "/" + get_file_name(meta_data.get_name());
			meta_data.set_path(file_path);
			stream.open(file_path, std::ios::out | std::ios::in | std::ios::binary);

			std::cout << "[D] " << file_path << " -> " << meta_data.get_name() << '\n';

			if (!stream.is_open()) {
				std::cerr << "Failed to open: " << file_path << '\n';
				exit(-1);
			}

			// Find the file extension
			/// @todo This is disgusting
			std::string extension = "";
			if (has_file_extension(meta_data.get_name())) {
				extension = get_file_extension(meta_data.get_name());

				ValTree plugin = v.query("plugins" + extension);
				if (!plugin.isNull()) {
					process(plugin.getStr(), stream);
				}
			}

			/// @todo: Make as seperate branch that takes te output from the plugin instead of the raw file stream
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
