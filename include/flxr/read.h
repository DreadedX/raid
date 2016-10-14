#pragma once
//----------------------------------------------
#include <fstream>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	void check_crc(Container& container);
	void read_header(Container& container);
	void read_index(Container& container);
	//----------------------------------------------
}
