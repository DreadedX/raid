#pragma once
//----------------------------------------------
#include <fstream>
#include <functional>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	std::fstream& write_header(std::fstream& stream, Container& container);
	std::fstream& write_index(std::fstream& stream, Container& container);
	std::fstream& write_data(std::fstream& stream, Container& container, int level,
			std::function<void(const std::string&, const uint64)> on_init = nullptr,
			std::function<void(const uint64)> on_update = nullptr,
			std::function<void()> on_finish = nullptr);
	std::fstream& write_crc(std::fstream& stream);
	//----------------------------------------------
}
