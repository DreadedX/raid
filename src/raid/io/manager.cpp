//----------------------------------------------
#include "typedef.h"
#include "raid/io/manager.h"
#include "flxr/spec.h"
#include "flxr/exceptions.h"

#include "logger.h"
//----------------------------------------------
/// @todo Make this not platform specific
raid::FileManager::FileManager() {
	// Create en pointer to a container
	#ifndef ANDROID
		std::unique_ptr<flxr::Container> container = std::make_unique<flxr::Container>("test.flx");
	#else
		std::unique_ptr<flxr::Container> container = std::make_unique<flxr::Container>("/storage/emulated/0/Android/data/nl.mtgames.daidalos/files/test.flx");
	#endif

	// try {
		container->check_crc();
		container->read_header();
		container->read_index();

		for(auto& meta_data : (*container).get_index()) {
			add_file(meta_data);
		}

		containers.emplace_back(std::move(container));
	// } catch(flxr::bad_file& e) {
	// 	warning << e.what() << '\n';
	// }
}
//----------------------------------------------
flxr::MetaData& raid::FileManager::get_file(std::string file_name) {
	/// @todo Optimize this
	// Search for file and return it
	for (auto& file : files) {
		if (file.get_name() == file_name) {
			return file;
		}
	}

	warning << "Unable to find file: " << file_name << '\n';
	throw std::runtime_error("Unable to find file");

	/// @todo This should be an exception, maybe even return a default object or something
	// exit(-1);
}
//----------------------------------------------
/// @todo Prevent duplication
void raid::FileManager::add_file(flxr::MetaData& meta_data) {
	debug << "Adding file: " << meta_data.get_name() << '\n';
	files.push_back(meta_data);
}
//----------------------------------------------
