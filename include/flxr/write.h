#pragma once
//----------------------------------------------
#include <fstream>
#include <functional>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	void write_header(Container& container);
	void write_index(Container& container);
	void write_data(Container& container, File& file, std::iostream& source,
			std::function<void(const std::string&, const uint64)> on_init = nullptr,
			std::function<void(const uint64)> on_update = nullptr,
			std::function<void(const uint64)> on_finish = nullptr);
	void write_crc(Container& container);
	//----------------------------------------------
}
