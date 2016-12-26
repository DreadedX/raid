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
#include "flxr/binary_helper.h"
#include "flxr/exceptions.h"

#include "logger.h"

using namespace raid;
//----------------------------------------------
class Texture : public Resource {
	/// Make sure that the factory can access private elements
	/// @todo Make this automated somehow
	template <class T> friend std::shared_ptr<T> ResourceManager::factory(std::string resource_name);

	private:
		/// Load a texture asset
		void load(std::string resource_name) override {
			std::stringstream stream;
			auto& file_manager = Engine::instance().get_file_manager();
			auto& data = file_manager.get_file(resource_name);
			try {
				flxr::read_data(data, stream);
			} catch(flxr::bad_compression_type& e) {
				warning << e.what() << '\n';
			}

			debug << "From Texture: " << resource_name << "\n";
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

	debug << file_manager.get_file("test/textures/test.png").get_name() << '\n';
	auto data1 = file_manager.get_file("test/textures/test.png");
	// We might want to add a version of read data that reads to an array/vector...
	// Or improve memstream to also be written to...
	std::stringstream stream1;
	flxr::read_data(data1, stream1);
	for (unsigned int i = 0; i < stream1.str().length(); ++i) {
		byte dat;
		flxr::read(stream1, dat);
		debug << std::hex << (int)dat << ' ';
	}
	debug << '\n';

	auto& resource = Engine::instance().get_resource();

	auto& data2 = file_manager.get_file("test/hello.txt");
	debug << data2.get_name() << '\n';
	std::stringstream stream2;
	flxr::read_data(data2, stream2);
	debug << stream2.str() << '\n';
	//----------------------------------------------
	//-Resources------------------------------------
	auto texture1 = resource.factory<Texture>("test/textures/test.png");
	auto texture2 = resource.factory<Texture>("test/hello.txt");
	auto texture11 = resource.factory<Texture>("test/textures/test.png");
	{
		auto texture3 = resource.factory<Texture>("test/file");
	}
	resource.debug_list();
	auto texture33 = resource.factory<Texture>("test/file");
	
	resource.debug_list();

	// Basic game loop
	while(!graphics.should_window_close()) {
		graphics.poll_events();
		graphics.swap_buffers();
	}

	//----------------------------------------------
	debug << "End\n";
}
//----------------------------------------------
