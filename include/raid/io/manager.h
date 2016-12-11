#pragma once
//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>

#include "flxr/spec.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	/// @todo Implement FLX-container loading
	/// @todo Also allow loading from filesystem while debugging
	/// @todo This should be a static class
	class FileManager {

		public:
			/// Create a list of all the files
			FileManager();

			// Returns reference to file, DO NO STORE THIS, when reloading this can become invalid on reload or addition of more files
			flxr::MetaData& get_file(std::string file_name);

		private:
			/// List of all containers
			std::vector<std::unique_ptr<flxr::Container>> containers;
			/// List of all known files
			std::vector<flxr::MetaData> files;

			/// Add file to list of known files
			void add_file(flxr::MetaData& meta_data);
	};
	//----------------------------------------------
}
