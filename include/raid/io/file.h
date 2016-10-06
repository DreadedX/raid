#pragma once
//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>

#include "typedef.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	/// @Todo: Implement FLX-file loading
	/// @Todo: Also allow loading from filesystem while debugging
	class File {
		public:
			/// Set the file name on initialization
			File(std::string m_name);

			/// Return the data contained in the file
			std::shared_ptr<std::vector<byte>> get_data();

			/// Returns the file name
			std::string get_name();

		private:
			/// File name
			std::string name;
			/// Weak ptr to the data
			std::weak_ptr<std::vector<byte>> data;
	};
	//----------------------------------------------
};
