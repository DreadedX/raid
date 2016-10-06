//----------------------------------------------
#include "typedef.h"
#include "raid/io/manager.h"
//----------------------------------------------
raid::FileManager::FileManager() {
	add_file("core/texture1");
	add_file("core/texture2");
}
//----------------------------------------------
std::shared_ptr<raid::File> raid::FileManager::get_file(std::string file_name) {
	/// @Todo: Optimize this
	// Search for file and return it
	for (std::shared_ptr<File> file : files) {
		if (file->get_name() == file_name) {
			return file;
		}
	}

	std::cout << "Unable to find file: " << file_name << '\n';

	/// @Todo: Maybe do something with exceptions
	// If the file was not found, return nullptr
	return nullptr;
}
//----------------------------------------------
/// @Todo: Prevent duplication
void raid::FileManager::add_file(std::string file_name) {
	std::cout << "Adding file: " << file_name << '\n';
	files.push_back(std::make_shared<File>(file_name));
};
//----------------------------------------------
