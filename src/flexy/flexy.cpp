//----------------------------------------------
#include <iostream>
#include <iomanip>
#include <memory>

#if __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#pragma message "Compiler does not support experimental/filesystem"
#endif

#if __has_include(<dlfcn.h>)
	#include <dlfcn.h>
#elif __has_include(<windows.h>)
	#include <windows.h>
#else
	#pragma message "The plugin system is not supported on this platform"
#endif

#include "sol.hpp"

#include "flexy/helper.h"
#include "flexy/interface.h"
#include "flexy/plugin.h"

/// @note Do not put this above flexy includes, somehow breaks
/// @todo Figure this out
#include "flxr/spec.h"
#include "flxr/exceptions.h"

#include "logger.h"

using namespace flxr;
//----------------------------------------------
#if __has_include(<experimental/filesystem>)
// @todo Add abstraction for plugin loading
auto process(std::string plugin_name, std::string file_path) {
	typedef std::shared_ptr<std::iostream> (*process_pointer)(std::string file_path);
	#if __has_include(<dlfcn.h>)
		/// @todo Look in directory of binary, maybe set this from config
		std::string base_path = "";
		plugin_name = base_path + plugin_name + ".so";
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
		std::string base_path = "";
		plugin_name = base_path + plugin_name + ".dll";
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
void write_test(std::string config_path) {
	fs::path path(config_path);
	std::string base_path = path.parent_path().string() + '/';

	sol::state lua;
	// Make the compression enums available to the config script
	lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::package, sol::lib::io, sol::lib::string, sol::lib::table);
	lua.script(R"lua(package.path = ")lua" + base_path +  R"lua(?.lua;" .. package.path)lua");
	lua.script(R"lua(package.cpath = ")lua" + base_path +  R"lua(?.so;" .. package.cpath)lua");
	lua.create_named_table("flexy", "base_path", base_path, "compression", lua.create_table_with(
				"zlib", COMPRESSION::ZLIB,
				"raw", COMPRESSION::RAW,
				"on_disk", COMPRESSION::ON_DISK
				));
	// Load the config file
	sol::protected_function_result config_result = lua.do_file(config_path);

	if (config_result.valid() && ((sol::object)config_result).is<sol::table>()) {
		sol::table config = config_result;
		if (((sol::object)config["packages"]).is<sol::table>()) {
			sol::table packages = config["packages"];

			/// @todo Check if values are what we expect
			for (auto package_pair : packages) {
				if (package_pair.second.is<sol::table>()) {
					sol::table package = package_pair.second.as<sol::table>();

					// Default compression settings
					sol::table compression = lua.create_table_with("type", COMPRESSION::RAW);
					// If compression settings exist load them
					/// @todo This should be passed to the compression function
					if (((sol::object)package["compression"]).is<sol::table>() && ((sol::object)package["compression"]["type"]).is<COMPRESSION>()) {
						compression = package["compression"];
					} else {
						warning << "Package missing compression setting, using defaults: " << package_pair.first.as<std::string>() << '\n';
					}

					std::string name = package.get_or<std::string>("name", "default");
					/// @todo Add checking to this
					std::string package_path = package.get<std::string>("path");
					COMPRESSION type = compression["type"];
					Container container(name + ".flx");

					if (((sol::object)package["files"]).is<sol::table>()) {
						sol::table files = package["files"];
						for (auto file : files) {
							std::string file_name = name + "/" + file.second.as<std::string>();
							std::string path = package_path + "/" + file.second.as<std::string>();
							container.add_file( MetaData(file_name, path, container) );
						}
					} else {
						warning << "Package doen not contain any files or is malformed: " << package_pair.first.as<std::string>() << '\n';
					}
					container.configure(type, 9);
					container.empty_file();
					container.write_header();
					container.write_index();

					for (auto& meta_data : container.get_index()) {
						debug << meta_data.get_path() << " -> " << meta_data.get_name() << '\n';

						// Find the file extension and corresponding plugin
						std::string plugin = "";
						fs::path path(meta_data.get_name());
						if (path.extension() != "") {
							if (((sol::object)config["plugins"]).is<sol::table>()) {
								sol::table plugins = config["plugins"];
								for (auto plugin_pair : plugins) {
									if (plugin_pair.second.is<sol::table>()) {
										if (((sol::object)((sol::table)plugin_pair.second)["extensions"]).is<sol::table>()) {
											sol::table extensions = ((sol::table)plugin_pair.second)["extensions"];
											for (auto extension_pair : extensions) {
												if (extension_pair.second.is<std::string>()) {
													if ("." + extension_pair.second.as<std::string>() == path.extension()) {
														if (((sol::object)((sol::table)plugin_pair.second)["name"]).is<std::string>()) {
															plugin = ((sol::table)plugin_pair.second)["name"];
														} else {
															warning << "Plugin name is invalid " << plugin_pair.first.as<std::string>() << '\n';
														}
													}
												} else {
													warning << "Plugin extension is invalid: " << plugin_pair.first.as<std::string>() << '\n';
												}
											}
										} else {
											warning << "Plugin extension list is invalid: " << plugin_pair.first.as<std::string>() << '\n';
										}
									} else {
										warning << "Skipping invalid plugin: " << plugin_pair.first.as<std::string>() << '\n';
									}
								}
							} else {
								warning << "There are no plugins registered\n";
							}
						}

						// If a plugin was found run it, otherwise just load the file
						std::shared_ptr<std::iostream> stream;
						if (plugin != "") {
							stream = process(plugin, meta_data.get_path());
							debug << "Plugin: " << plugin << '\n';
						} else {
							stream = open_file(meta_data.get_path());
							debug << "No plugin\n";
						}

						meta_data.write_data(*stream, Progress::setup, Progress::draw, Progress::finish);
					}
					/// @todo Maybe combine this in container.finalize()
					container.write_index();
					container.write_crc();
				} else {
					warning << "Skipping invalid package: " << package_pair.first.as<std::string>() << '\n';
				}
			}
		} else {
			warning << "Packages does not exist or is malformed\n";
		}
	} else {
		sol::error error = config_result;
		warning << error.what() << '\n';
	}
}
//----------------------------------------------
void read_test(std::string config_path) {
	std::string base_path = config_path.substr(0, config_path.find_last_of("\\/")+1);

	sol::state lua;
	// Make the compression enums available to the config script
	lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::package, sol::lib::io, sol::lib::string, sol::lib::table);
	lua.script(R"lua(package.path = ")lua" + base_path +  R"lua(?.lua;" .. package.path)lua");
	lua.script(R"lua(package.cpath = ")lua" + base_path +  R"lua(?.so;" .. package.cpath)lua");
	lua.create_named_table("flexy", "base_path", base_path, "compression", lua.create_table_with(
				"zlib", COMPRESSION::ZLIB,
				"raw", COMPRESSION::RAW,
				"on_disk", COMPRESSION::ON_DISK
				));
	// Load the config file
	sol::protected_function_result config_result = lua.do_file(config_path);

	if (config_result.valid() && ((sol::object)config_result).is<sol::table>()) {
		sol::table config = config_result;
		if (((sol::object)config["packages"]).is<sol::table>()) {
			sol::table packages = config["packages"];

			/// @todo Check if values are what we expect
			for (auto package_pair : packages) {
				if (package_pair.second.is<sol::table>()) {
					sol::table package = package_pair.second.as<sol::table>();

					std::string name = package.get_or<std::string>("name", "default");
					Container container(name + ".flx");

					try {
						container.check_crc();
						container.read_header();
						container.read_index();

						for (auto& meta_data : container.get_index()) {
							debug << meta_data.get_name() << " " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(meta_data.get_size())/1000/1000 << " MB compressed\n";
						}

						for (auto& meta_data : container.get_index()) {
							std::stringstream stream;
							try {
								meta_data.read_data(stream, Progress::setup, Progress::draw, Progress::finish);
							} catch(flxr::bad_compression_type& e) {
								warning << e.what() << '\n';
							}
							// debug << stream.str() << '\n';
						}
					} catch(flxr::bad_file& e) {
						warning << e.what() << '\n';
					}
				}
			}
		}
	}
}
//----------------------------------------------
int main(int argc, const char* argv[]) {
	try {
		if (argc == 2) {
			std::string config_path(argv[1]);
			debug << "===WRITE===\n";
			write_test(config_path);
			debug << "===READ===\n";
			read_test(config_path);
		} else {
			warning << "Incorrect amount of arguments\n";
		}
	} catch (std::exception& e) {
		warning << e.what() << '\n';
	}
}
#else
int main() {
	warning << "experimental/filesystem not available on this system\n";
}
#endif
