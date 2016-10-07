#pragma once
//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>

#include "raid/io/file.h"
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

			// Returns ptr to file
			std::shared_ptr<File> get_file(std::string file_name);

		private:
			/// List of all known files
			std::vector< std::shared_ptr<File> > files;

			/// Add file to list of known files
			void add_file(std::string file_name);
	};
	//----------------------------------------------
};
