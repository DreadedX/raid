//----------------------------------------------
#include <iostream>
#include <iomanip>
#include <memory>

#if __has_include(<dlfcn.h>)
	#include <dlfcn.h>
#elif __has_include(<windows.h>)
	#include <windows.h>
#else
	#pragma message "The plugin system is not supported on this platform"
#endif

#include "flexy/ValTree.h"
#include "flexy/helper.h"
#include "flexy/interface.h"
#include "flexy/plugin.h"

/// @note Do not put this above flext includes, somehow breaks
/// @todo Figure this out
#include "flxr/write.h"
#include "flxr/read.h"
#include "flxr/exceptions.h"

#include "logger.h"

using namespace flxr;
//----------------------------------------------
// @todo Add abstraction for plugin loading
auto process(std::string plugin_name, std::string file_path) {
	typedef std::shared_ptr<std::iostream> (*process_pointer)(std::string file_path);
	#if __has_include(<dlfcn.h>)
		/// @todo Look in directory of binary, maybe set this from config
		std::string base_path = "../bin/";
		plugin_name = base_path + "lib" + plugin_name + ".so";
		void* handle = dlopen(plugin_name.c_str(), RTLD_NOW);
		char* error = dlerror();
		if (error) {
			warning << error << '\n';
			exit(-1);
		}
		delete[] error;
		error = nullptr;

		process_pointer process = (process_pointer)dlsym(handle, "process");
		error = dlerror();
		if (error) {
			warning << error << '\n';
			exit(-1);
		}
		delete[] error;
		error = nullptr;
		return process(file_path);
	#elif __has_include(<windows.h>)
		// Somehow errors but still works...
		std::string base_path = "./";
		plugin_name = base_path + "lib" + plugin_name + ".dll";
		debug << plugin_name << '\n';
		HINSTANCE dll = LoadLibraryA(plugin_name.c_str());
		DWORD error = GetLastError();
		if (error) {
			warning << "LoadLibrary: "  << error << '\n';
			// exit(-1);
		}
		process_pointer process = (process_pointer)GetProcAddress(dll, "process");
		error = GetLastError();
		if (error) {
			warning << "GetProcAddress: "  << error << '\n';
			// exit(-1);
		}
		return process(file_path);
	#else
		Plugin().process(file_path);
		return open_file(file_path);
	#endif
}
//----------------------------------------------
void write_test() {
	/// @todo The path to the config needs to be an commandline argument, and everything needs to be relative to the config file
	std::string base_path = "../../";

	ValTree v;
	v.parse(base_path + "assets/config.flxr");

	for (const auto& package : v.getChild("packages")) {

		debug << "WRITE TEST\n";

		// Refactor to setup function
		Container container(package.getKey() + ".flx");
		{
			for (auto file : package.getChild("files")) {
				std::string name = package.getKey() + "/" + file.getKey();
				std::string path = base_path + package.getChild("path").getStr() + "/" + get_file_name(name);
				container.add_file( MetaData(name, path, container) );
			}

			COMPRESSION compression_type = COMPRESSION::RAW;
			try {
				compression_type = get_compression_type(package);
			} catch(bad_compression_type& e) {
				warning << e.what() << "\nUsing RAW...\n";
			}

			container.configure(compression_type, package.query("compression.level").getInt());
			debug << "Compression type: " << compression_type << ", level: " << package.query("compression.level").getInt() << '\n';

			container.empty_file();
		}
		write_header(container);
		write_index(container);

		for (auto& meta_data : container.get_index()) {
			debug << meta_data.get_path() << " -> " << meta_data.get_name() << '\n';

			// Find the file extension and corresponding plugin
			ValTree plugin;
			if (has_file_extension(meta_data.get_name())) {
				plugin = v.query("plugins" + get_file_extension(meta_data.get_name()));
			}

			// If a plugin was found run it, otherwise just load the file
			std::shared_ptr<std::iostream> stream;
			if (!plugin.isNull()) {
				stream = process(plugin.getStr(), meta_data.get_path());
			} else {
				stream = open_file(meta_data.get_path());
				debug << "No plugin\n";
			}

			write_data(meta_data, *stream, Progress::setup, Progress::draw, Progress::finish);
		}
		write_index(container);
		write_crc(container);
	}
}
//----------------------------------------------
void read_test() {
	/// @todo The path to the config needs to be an commandline argument, and everything needs to be relative to the config file
	std::string base_path = "../../";

	ValTree v;
	v.parse(base_path + "assets/config.flxr");

	for (const auto& package : v.getChild("packages")) {

		debug << "READ TEST\n";

		Container container(package.getKey() + ".flx");

		try {
			check_crc(container);
			read_header(container);
			read_index(container);

			for (auto& meta_data : container.get_index()) {
				debug << meta_data.get_name() << " " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(meta_data.get_size())/1000/1000 << " MB compressed\n";
			}

			for (auto& meta_data : container.get_index()) {
				std::stringstream stream;
				try {
					read_data(meta_data, stream);
				} catch(flxr::bad_compression_type& e) {
					warning << e.what() << '\n';
				}
				// debug << stream.str() << '\n';
			}

			debug << "==============================\n";
		} catch(flxr::bad_file& e) {
			warning << e.what() << '\n';
		}
	}
}
//----------------------------------------------
int main() {
	debug << "This is a debug message\nMore text\n";
	message << "This is a debug message\n";
	warning << "This is a warning message\n";
	write_test();
	read_test();
}
