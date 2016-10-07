//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

#include "typedef.h"
#include "raid/resource.h"
#include "raid/io/manager.h"
#include "raid/graphics/kit.h"
#include "raid/graphics/opengl/opengl.h"

using namespace raid;
//----------------------------------------------
class TextureResource : public BaseResource {
	/// Make sure that the factory can access private elements
	/// @todo Make this automated somehow
	template <class T> friend std::shared_ptr<T> Resource::factory(std::string resource_name);

	private:
		/// Load a texture asset
		void load(std::string resource_name) override {
			// auto textureData = fileManager.get_file(resource_name)->get_data();
		}
};
//----------------------------------------------
int main() {
	//-Files----------------------------------------
	GraphicsKit graphics_kit(std::make_unique<OpenGLImpl>());
	graphics_kit.create_window();
	//----------------------------------------------
	//-Files----------------------------------------
	/// @todo Upgrade this thing to use some of the fancy stuff used by resources
	FileManager file_manager;

	auto data1 = file_manager.get_file("core/texture1")->get_data();
	for (auto _byte : *data1) {
		std::cout << int(_byte) << ' ';
	}
	std::cout << '\n';

	auto data2 = file_manager.get_file("core/texture1")->get_data();
	//----------------------------------------------
	//-Resources------------------------------------
	auto texture1 = Resource::factory<TextureResource>("core/texture1");
	auto texture2 = Resource::factory<TextureResource>("core/texture2");
	auto texture11 = Resource::factory<TextureResource>("core/texture1");
	{
		auto texture3 = Resource::factory<TextureResource>("core/texture3");
	}
	auto texture33 = Resource::factory<TextureResource>("core/texture3");
	
	Resource::debug_list();
	//----------------------------------------------
	std::cout << "End\n";
}
//----------------------------------------------
