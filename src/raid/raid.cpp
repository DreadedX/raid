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
class Tile {
	public:
		Tile(float x, float y, float width, float height) : _x(x), _y(y), _width(width), _height(height) {
			_texture = Engine::instance().get_platform()->load_texture("test/textures/grass.png");
			_shader = Engine::instance().get_platform()->load_shader("WIP");
		}

		void draw() {
			Engine::instance().get_platform()->draw_sprite(_x, _y, _width, _height, 0.0f, _texture, _shader);
		}

	private:
		// Replace width and heigt with a shape, which is also used for collision and knowing the size of the sprite that needs to be rendered
		float _x;
		float _y;
		float _width;
		float _height;
		std::shared_ptr<Texture> _texture;
		std::shared_ptr<Shader> _shader;
};
//----------------------------------------------
ENTRY {
	Engine::instance().set_platform(std::make_unique<PLATFORM_IMPL>(PLATFORM_ARGS));
	auto& platform = Engine::instance().get_platform();

	auto& file_manager = Engine::instance().get_file_manager();
	file_manager.add_container("test.flx");

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
			platform->test_setup();

			resource.debug_list();
			Tile tile(200.0f, 200.0f, 64.0f, 64.0f);
			resource.debug_list();
			Tile tile2(264.0f, 200.0f, 64.0f, 64.0f);
			resource.debug_list();
			
			while(platform->has_context() && !platform->should_window_close()) {
				platform->test_render();

				tile.draw();
				tile2.draw();

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
