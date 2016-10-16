#pragma once
//----------------------------------------------
#include <fstream>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	void read_header(Container& container);
	void read_index(Container& container);
	void read_data(Container& container, File& file, std::iostream& dest);
	void check_crc(Container& container);
	//----------------------------------------------
}
