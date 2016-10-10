#pragma once
//----------------------------------------------
#include <fstream>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	std::fstream& check_crc(std::fstream& stream);
	std::istream& read_header(std::istream& stream, Container& container);
	std::istream& read_index(std::istream& stream, Container& container);
	//----------------------------------------------
}
