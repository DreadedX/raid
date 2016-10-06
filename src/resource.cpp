//----------------------------------------------
#include "raid/resource.h"
//----------------------------------------------
std::unordered_map<std::string, std::weak_ptr<raid::BaseResource>> raid::Resource::list;
//----------------------------------------------
void raid::Resource::debug_list() {
	for(auto resource : list) {
		std::cout << '\t' << resource.first;

		if (!resource.second.expired()) {
			std::cout << " [valid]\n";
		} else {
			std::cout << " [expired]\n";
		}
	}
}
//----------------------------------------------
