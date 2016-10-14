#pragma once
//----------------------------------------------
#include <fstream>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	std::ostream& write_header(std::ostream& stream, Container& container);
	std::fstream& write_index(std::fstream& stream, Container& container);
	std::ostream& write_data(std::ostream& stream, Container& container, int level);
	std::fstream& write_crc(std::fstream& stream);
	//----------------------------------------------
}
