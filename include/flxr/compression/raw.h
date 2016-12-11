#pragma once
//----------------------------------------------
#include <functional>

#include "flxr/spec.h"
//----------------------------------------------
namespace flxr { namespace raw {
	//----------------------------------------------
	void write_data(MetaData& meta_data, std::iostream& source,
			std::function<void(const std::string&, const uint64)> on_init = nullptr,
			std::function<void(const uint64)> on_update = nullptr,
			std::function<void(const uint64)> on_finish = nullptr);
	void read_data(MetaData& meta_data, std::iostream& dest);
	//----------------------------------------------
}}
