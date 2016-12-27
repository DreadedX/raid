#pragma once
//----------------------------------------------
#include <exception>
//----------------------------------------------
namespace flxr {
	class bad_compression_type : public std::exception {
		public:
			virtual const char* what() const throw() {
				return "Invalid compression type";
			}
	};
	class bad_magic_type : public std::exception {
		public:
			virtual const char* what() const throw() {
				return "Invalid magic number";
			}
	};
	class bad_file : public std::exception {
		public:
			virtual const char* what() const throw() {
				return "File is corrupt";
			}
	};
	class not_implemented : public std::exception {
		public:
			virtual const char* what() const throw() {
				return "This function is not implemented";
			}
	};
}

