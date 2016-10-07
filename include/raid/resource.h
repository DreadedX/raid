#pragma once
//----------------------------------------------
#include <iostream>
#include <memory>
#include <unordered_map>
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	/// @todo Allow the reloading of resources
	class BaseResource {
		public:
			BaseResource() {
				std::cout << "Resource constructed\n";
			}

			~BaseResource() {
				std::cout << "Resource Deconstructed\n";
			}

		private:
			/// All resourcess need this function to load the actual resource.
			virtual void load(std::string resource_name) = 0;
	};
	//----------------------------------------------
	class Resource {
		public:
			/// @todo figure out a way to remove unloaded resources from the list automatically
			/// List containing all resources that are maybe loaded
			static std::unordered_map<std::string, std::weak_ptr<raid::BaseResource>> list;

			/// Print list of all loaded assets
			/// @todo This is temporary
			static void debug_list();

			/// Create new resource instance of type T and initialize it.
			template <class T> static std::shared_ptr<T> factory(std::string resource_name) {
				// Make sure the requested type is derived from BaseResource
				static_assert(std::is_base_of<BaseResource, T>::value, "T needs to be derived from Resource");

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
					auto ptr = std::weak_ptr<BaseResource>();
					ptr = std::dynamic_pointer_cast<BaseResource, T>(t);

					// Add the weak ptr to the resource list
					list.insert({resource_name, ptr});

					// Return the shared ptr
					return t;
				}

				std::cout << "Resource already in memory: " << resource_name << '\n';
				// The weak ptr was still valid, so we return a shared ptr to the asset
				return std::dynamic_pointer_cast<T, BaseResource>(list.find(resource_name)->second.lock());
			}
	};
	//----------------------------------------------
};
//----------------------------------------------
