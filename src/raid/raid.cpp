//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "typedef.h"
#include "raid/engine.h"
#include "raid/asset/texture.h"

#include "flxr/spec.h"
#include "flxr/binary_helper.h"
#include "flxr/exceptions.h"

#include "logger.h"

#include "raid/platform/platform.h"

using namespace raid;
//----------------------------------------------
class Dummy : public Resource {
	public:
		void load(std::string asset_name) override {
			debug << "Dummy asset: " << asset_name << '\n';
		}
};
//----------------------------------------------
ENTRY {
	// debug << "Engine init goed here\n";
	// LOGI("Engine init goes here");
	Engine::instance().set_platform(std::make_unique<PLATFORM_IMPL>(PLATFORM_ARGS));
	auto& platform = Engine::instance().get_platform();

	auto& file_manager = Engine::instance().get_file_manager();
	// #ifndef ANDROID
		file_manager.add_container("test.flx");
	// #else
		// file_manager.add_container("/storage/emulated/0/Android/data/nl.mtgames.daidalos/files/test.flx");
	// #endif

	auto& resource = Engine::instance().get_resource();

	auto& data2 = file_manager.get_file("test/hello.txt");
	debug << data2.get_name() << '\n';
	std::stringstream stream2;
	data2.read_data(stream2);
	debug << stream2.str() << '\n';
	//----------------------------------------------
	//-Resources------------------------------------
	auto texture2 = resource.get<Dummy>("test/hello.txt");
	{
		auto texture3 = resource.get<Dummy>("test/file");
	}
	resource.debug_list();
	auto texture33 = resource.get<Dummy>("test/file");
	
	resource.debug_list();

	while(!platform->should_window_close())  {

		platform->poll_events();

		if (platform->has_context()) {
			debug << "Graphics init goes here\n";
			// LOGI("Graphics init goes here");
			platform->create_window(1280, 720, "Daidalos Engine");
			auto shader = platform->test_setup();

			resource.debug_list();
			auto texture1 = platform->load_texture("test/textures/test.png");
			auto texture11 = platform->load_texture("test/textures/test.png");
			resource.debug_list();
			
			while(platform->has_context() && !platform->should_window_close()) {

				platform->test_render(shader);

				platform->poll_events();
				platform->swap_buffers();
			}

			debug << "Graphics cleanup code goed here\n";
			// LOGI("Graphics cleanup code goed here");
			platform->terminate();
		}
	}
	resource.debug_list();
	// LOGI("Engine cleanup code goed here");
	debug << "Engine cleanup code goed here\n";
}
