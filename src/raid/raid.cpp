//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "typedef.h"
#include "raid/engine.h"
// #include "raid/resource.h"
// #include "raid/io/manager.h"
// #include "raid/graphics/manager.h"
#include "raid/graphics/opengl/opengl.h"

#include "flxr/read.h"

using namespace raid;
//----------------------------------------------
class Texture : public Resource {
	/// Make sure that the factory can access private elements
	/// @todo Make this automated somehow
	template <class T> friend std::shared_ptr<T> ResourceManager::factory(std::string resource_name);

	private:
		/// Load a texture asset
		void load(std::string) override {
			// auto textureData = fileManager.get_file(resource_name)->get_data();
		}
};
//----------------------------------------------
int main() {
	//-Graphics-------------------------------------
	GraphicsManager& graphics = Engine::instance().get_graphics();
	graphics.set_impl<OpenGL>();
	graphics.create_window(1280, 720, "Daidalos Engine");
	//----------------------------------------------
	//-Files----------------------------------------
	/// @todo Upgrade this thing to use some of the fancy stuff used by resources
	auto& file_manager = Engine::instance().get_file_manager();

	std::cout << file_manager.get_file("test/textures/test.png").get_name() << '\n';
	// auto data1 = file_manager.get_file("core/texture1")->get_data();
	// for (auto _byte : *data1) {
	// 	std::cout << int(_byte) << ' ';
	// }
	// std::cout << '\n';

	auto& resource = Engine::instance().get_resource();

	auto& data2 = file_manager.get_file("test/hello.txt");
	std::cout << data2.get_name() << '\n';
	std::stringstream stream;
	flxr::read_data(data2, stream);
	std::cout << stream.str() << '\n';
	//----------------------------------------------
	//-Resources------------------------------------
	auto texture1 = resource.factory<Texture>("core/texture1");
	auto texture2 = resource.factory<Texture>("core/texture2");
	auto texture11 = resource.factory<Texture>("core/texture1");
	{
		auto texture3 = resource.factory<Texture>("core/texture3");
	}
	auto texture33 = resource.factory<Texture>("core/texture3");
	
	resource.debug_list();

	// Basic game loop
	while(!graphics.should_window_close()) {
		graphics.poll_events();
		graphics.swap_buffers();
	}

	//----------------------------------------------
	std::cout << "End\n";
}
//----------------------------------------------
