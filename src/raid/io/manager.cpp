//----------------------------------------------
#include "typedef.h"
#include "raid/io/manager.h"
#include "flxr/spec.h"
#include "flxr/read.h"
#include "flxr/exceptions.h"

#include "logger.h"
//----------------------------------------------
raid::FileManager::FileManager() {
	// Create en pointer to a container
	std::unique_ptr<flxr::Container> container = std::make_unique<flxr::Container>("test.flx");

	try {
		// THIS DOES NOT WORK ON WINDOWS
		flxr::check_crc(*container);
		flxr::read_header(*container);
		flxr::read_index(*container);

		for(auto& meta_data : (*container).get_index()) {
			add_file(meta_data);
		}

		containers.emplace_back(std::move(container));
	} catch(flxr::bad_file& e) {
		warning << e.what() << '\n';
	}
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

	/// @todo This should be an exception, maybe even return a default object or something
	exit(-1);
}
//----------------------------------------------
/// @todo Prevent duplication
void raid::FileManager::add_file(flxr::MetaData& meta_data) {
	debug << "Adding file: " << meta_data.get_name() << '\n';
	files.push_back(meta_data);
}
//----------------------------------------------
