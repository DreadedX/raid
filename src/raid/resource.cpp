//----------------------------------------------
#include "raid/resource.h"
#include "raid/engine.h"

#include "logger.h"

// std::unordered_map<std::string, std::weak_ptr<raid::BaseResource>> raid::Resource::list;
//----------------------------------------------
void raid::ResourceManager::debug_list() {
	for(auto resource : list) {
		debug << '\t' << resource.first;

		if (!resource.second.expired()) {
			debug << " [valid]\n";
		} else {
			debug << " [expired]\n";
		}
	}
}
//----------------------------------------------
