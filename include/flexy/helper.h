#pragma once
//----------------------------------------------
#include <string>
#include <memory>
#include "flxr/spec.h"

#include "logger.h"
//----------------------------------------------
inline std::shared_ptr<std::iostream> open_file(std::string file_path) {
	std::shared_ptr<std::iostream> stream = std::make_shared<std::fstream>();
	std::static_pointer_cast<std::fstream>(stream)->open(file_path, std::ios::out | std::ios::in | std::ios::binary);
	if (!std::static_pointer_cast<std::fstream>(stream)->is_open()) {
		warning << "Failed to open: " << file_path << '\n';
		/// @todo This should be an exception
		exit(-1);
	}

	return stream;
}
//----------------------------------------------
