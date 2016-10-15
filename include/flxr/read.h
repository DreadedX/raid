#pragma once
//----------------------------------------------
#include <fstream>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	void read_header(Container& container);
	void read_index(Container& container);
	void read_data(Container& container);
	void check_crc(Container& container);
	//----------------------------------------------
}
