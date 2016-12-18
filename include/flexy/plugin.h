#pragma once
//----------------------------------------------
#include <iostream>
#include <memory>

#include "flexy/helper.h"
//----------------------------------------------
class Plugin {

	public:
		virtual std::shared_ptr<std::iostream> process(std::string file_path) {
			warning << "Plugin is not implemented\n";
			return open_file(file_path);
		};
};
//----------------------------------------------
#define share_plugin(plugin) extern "C" {std::shared_ptr<std::iostream> process(std::string file_path) { return plugin ().process(file_path); }}
//----------------------------------------------
