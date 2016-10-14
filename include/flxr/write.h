#pragma once
//----------------------------------------------
#include <fstream>
#include <functional>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	std::ostream& write_header(std::ostream& stream, Container& container);
	std::fstream& write_index(std::fstream& stream, Container& container);
	std::ostream& write_data(std::ostream& stream, Container& container, int level,
			std::function<void(const std::string&, const uint64)> on_init = nullptr,
			std::function<void(const uint64)> on_update = nullptr,
			std::function<void()> on_finish = nullptr);
	std::fstream& write_crc(std::fstream& stream);
	//----------------------------------------------
}
