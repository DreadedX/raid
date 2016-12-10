//----------------------------------------------
#include <iostream>
#include <iomanip>
#include <dlfcn.h>
#include <memory>

#include "flexy/ValTree.h"
#include "flexy/helper.h"
#include "flexy/config.h"
#include "flexy/interface.h"

/// @note Do not put this above flext includes, somehow breaks
/// @todo Figure this out
#include "flxr/write.h"
#include "flxr/read.h"

using namespace flxr;
//----------------------------------------------
// @todo Add abstraction for plugin loading
auto process(std::string plugin_name, std::shared_ptr<std::iostream> stream) {
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

	typedef std::shared_ptr<std::iostream> (*process_pointer)(std::shared_ptr<std::iostream> stream);
	process_pointer process = (process_pointer)dlsym(handle, "process");
	error = dlerror();
	if (error) {
		std::cerr << error << '\n';
		exit(-1);
	}
	delete[] error;
	error = nullptr;

	return process(stream);
}
//----------------------------------------------
void write_test() {

	/// @todo The path to the config needs to be an commandline argument, and everything needs to be relative to the config file
	std::string base_path = "../../";

	ValTree v;
	v.parse(base_path + "assets/config.flxr");

	for (const auto& package : v.getChild("packages")) {

		std::cout << "[D] " << "WRITE TEST\n";

		// Refactor to setup function
		Container container(package.getKey() + ".flx");
		{
			for (auto file : package.getChild("files")) {
				container.add_file( MetaData(package.getKey() + "/" + file.getKey()) );
			}

			container.configure(get_compression_type(package), package.query("compression.level").getInt());
			std::cout << "[D] " << "Compression type: " << get_compression_type(package) << ", level: " << package.query("compression.level").getInt() << '\n';

			container.empty_file();
		}
		write_header(container);
		write_index(container);

		for (auto& meta_data : container.get_index()) {
			std::shared_ptr<std::iostream> stream = std::make_shared<std::fstream>();
			std::string file_path = base_path + package.getChild("path").getStr() + "/" + get_file_name(meta_data.get_name());
			meta_data.set_path(file_path);
			std::static_pointer_cast<std::fstream>(stream)->open(file_path, std::ios::out | std::ios::in | std::ios::binary);

			std::cout << "[D] " << file_path << " -> " << meta_data.get_name() << '\n';

			if (!std::static_pointer_cast<std::fstream>(stream)->is_open()) {
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
					stream = process(plugin.getStr(), stream);
				}
			}

			write_data(container, meta_data, *stream, Progress::setup, Progress::draw, Progress::finish);
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
			std::cout << stream.str() << '\n';
		}

		std::cout << "==============================\n";
	}
}
//----------------------------------------------
int main() {
	write_test();
	read_test();
}
