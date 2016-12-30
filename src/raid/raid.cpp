//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "typedef.h"
#include "raid/engine.h"

#include "flxr/spec.h"
#include "flxr/binary_helper.h"
#include "flxr/exceptions.h"

#include "logger.h"

#include "raid/platform/platform.h"

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
				data.read_data(stream);
			} catch(flxr::bad_compression_type& e) {
				warning << e.what() << '\n';
			}

			debug << "From Texture: " << resource_name << "\n";
		}
};
//----------------------------------------------
ENTRY {
	// debug << "Engine init goed here\n";
	// LOGI("Engine init goes here");
	auto& platform = Engine::instance().get_platform();
	platform.set_impl(std::make_unique<PLATFORM_IMPL>(PLATFORM_ARGS));

	auto& file_manager = Engine::instance().get_file_manager();

	debug << file_manager.get_file("test/textures/test.png").get_name() << '\n';
	auto data1 = file_manager.get_file("test/textures/test.png");
	// We might want to add a version of read data that reads to an array/vector...
	// Or improve memstream to also be written to...
	std::stringstream stream1;
	data1.read_data(stream1);
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
	data2.read_data(stream2);
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

	bool was_running = false;

	// Setup loop
	while(!platform.should_window_close())  {

		platform.poll_events();

		if (platform.has_context()) {
			// debug << "Graphics init goes here\n";
			// LOGI("Graphics init goes here");
			platform.create_window(1280, 720, "Daidalos Engine");
			platform.test_setup();
		}

		// // If app->window exists start the main game loop
		while(platform.has_context() && !platform.should_window_close()) {

			platform.test_render();

			platform.poll_events();
			platform.swap_buffers();

			was_running = true;
		}

		if (was_running) {
			// debug << "Graphics cleanup code goed here\n";
			// LOGI("Graphics cleanup code goed here");
			platform.terminate();
			was_running = false;
		}
	}
	// LOGI("Engine cleanup code goed here");
}
