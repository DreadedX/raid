#pragma once
//----------------------------------------------
#include <iostream>
#include <memory>
#include <unordered_map>
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	/// @todo Allow the reloading of resources
	class Resource {
		public:
			Resource() {
				std::cout << "Resource constructed\n";
			}

			virtual ~Resource() {
				std::cout << "Resource Deconstructed\n";
			}

		private:
			/// All resourcess need this function to load the actual resource.
			virtual void load(std::string resource_name) = 0;
	};
	//----------------------------------------------
	class ResourceManager {
		public:

			/// Print list of all loaded assets
			/// @todo This is temporary
			void debug_list();

			/// Create new resource instance of type T and initialize it.
			template <class T> std::shared_ptr<T> factory(std::string resource_name) {
				// Make sure the requested type is derived from Resource
				static_assert(std::is_base_of<Resource, T>::value, "T needs to be derived from Resource");

				// Check if the resource is already loaded and still valid
				if (list.count(resource_name) == 0 || list.find(resource_name)->second.expired())
				{

					// If the key exists it means it was expired
					if (list.count(resource_name) != 0) {
						std::cout << "Resource expired: " << resource_name << '\n';

						list.erase(list.find(resource_name));
					}

					std::cout << "Resource not in memory: " << resource_name << '\n';

					// Create new shared ptr
					std::shared_ptr<T> t = std::make_shared<T>();
					// Call the resources load function
					t->load(resource_name);

					// Create a weak ptr to the resource
					auto ptr = std::weak_ptr<Resource>();
					ptr = std::dynamic_pointer_cast<Resource, T>(t);

					// Add the weak ptr to the resource list
					list.insert({resource_name, ptr});

					// Return the shared ptr
					return t;
				}

				std::cout << "Resource already in memory: " << resource_name << '\n';
				// The weak ptr was still valid, so we return a shared ptr to the asset
				return std::dynamic_pointer_cast<T, Resource>(list.find(resource_name)->second.lock());
			}
		private:
			/// @todo figure out a way to remove unloaded resources from the list automatically
			/// List containing all resources that are maybe loaded
			std::unordered_map<std::string, std::weak_ptr<raid::Resource>> list;
	};
	//----------------------------------------------
}
//----------------------------------------------
