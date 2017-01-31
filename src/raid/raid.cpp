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
class Button {
	public:
		Button(int x, int y, int width, int height, std::string texture_name) : _x(x), _y(y), _width(width), _height(height) {
			_texture = Engine::instance().get_platform()->load_texture(texture_name);
			_shader = Engine::instance().get_platform()->load_shader("BUTTON");
		}

		void draw() {
			Engine::instance().get_platform()->draw_sprite(_x, _y, _width, _height, 0.0f, _texture, _shader);

			if (Engine::instance().get_platform()->is_pressed(_x, _y, _width, _height)) {
				Engine::instance().get_platform()->draw_sprite(_x+200, _y-200, _width*2, _height*2, 0.0f, _texture, _shader);
			}
		}

	private:
		std::shared_ptr<Texture> _texture;
		std::shared_ptr<Shader> _shader;
		int _x;
		int _y;
		int _width;
		int _height;
};
//----------------------------------------------
class Tile {
	public:
		Tile(std::string texture_name) {
			_texture = Engine::instance().get_platform()->load_texture(texture_name);
		}
		Tile(std::string texture_name, std::string shader_name) {
			_texture = Engine::instance().get_platform()->load_texture(texture_name);
			_shader = Engine::instance().get_platform()->load_shader(shader_name);
		}

		auto& get_texture() { return _texture; }

	private:
		// We need to store a "shape", this is going to be used for collision.
		// Each tile by definition is a square
		std::shared_ptr<Texture> _texture;
		// Only use the custom shader is it is specified
		std::shared_ptr<Shader> _shader;
};
//----------------------------------------------
class Chunk {
	public:
		Chunk(int x, int y) : chunk_x(x), chunk_y(y) {
			for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; ++i) {
				// Make the top layer a differnt texture
				if (i < CHUNK_SIZE) {
					tiles.emplace_back("test/textures/grass.png");
				} else {
					tiles.emplace_back("debug");
				}
			}
			_shader = Engine::instance().get_platform()->load_shader("TEST_SHADER");
		}

		int get_index(int x, int y) {
			return y + x*CHUNK_SIZE;
		}


		void draw() {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				for (int y = 0; y < CHUNK_SIZE; y++) {
					Engine::instance().get_platform()->draw_sprite(x*TILE_SIZE + chunk_x*CHUNK_SIZE*TILE_SIZE, y*TILE_SIZE + chunk_y*CHUNK_SIZE*TILE_SIZE, TILE_SIZE, TILE_SIZE, 0.0f,
							get_tile(x, y).get_texture(), _shader);
				}
			}
		}

	private:
		Tile &get_tile(int x, int y) {
			return tiles[x + y*CHUNK_SIZE];
		}

		static const int CHUNK_SIZE = 8;
		static const int TILE_SIZE = 16*4;
		int chunk_x = 0;
		int chunk_y = 0;
		std::vector<Tile> tiles;
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
			platform->create_window(1920, 1080, "Daidalos Engine");
			platform->test_setup();

			Chunk chunk1(0,0);
			Chunk chunk2(1,0);

			Button button(100, 800, 100, 100, "test/textures/grass.png");
			
			while(platform->has_context() && !platform->should_window_close()) {
				platform->test_render();

				chunk1.draw();
				chunk2.draw();

				button.draw();

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
