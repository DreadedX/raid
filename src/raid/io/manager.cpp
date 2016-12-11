//----------------------------------------------
#include "typedef.h"
#include "raid/io/manager.h"
#include "flxr/spec.h"
#include "flxr/read.h"
//----------------------------------------------
raid::FileManager::FileManager() {
	// Create en pointer to a container
	std::unique_ptr<flxr::Container> container = std::make_unique<flxr::Container>("test.flx");

	flxr::check_crc(*container);
	flxr::read_header(*container);
	flxr::read_index(*container);
    //
	std::cout << "Files in 'test.flx'\n";
	for(auto& meta_data : (*container).get_index()) {
		std::cout << meta_data.get_name() << '\n';
		add_file(meta_data);
	}

	containers.emplace_back(std::move(container));

	// add_file("core/texture1");
	// add_file("core/texture2");
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

	std::cout << "Unable to find file: " << file_name << '\n';

	/// @todo This should be an exception, maybe even return a default object or something
	exit(-1);
}
//----------------------------------------------
/// @todo Prevent duplication
void raid::FileManager::add_file(flxr::MetaData& meta_data) {
	std::cout << "Adding file: " << meta_data.get_name() << '\n';
	files.push_back(meta_data);
}
//----------------------------------------------
