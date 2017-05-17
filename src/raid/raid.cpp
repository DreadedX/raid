//----------------------------------------------
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>

#include "typedef.h"
#include "raid/engine.h"
#include "raid/asset/texture.h"
#include "raid/asset/vorbis.h"

#include "flxr/spec.h"
#include "flxr/binary_helper.h"
#include "flxr/exceptions.h"

#include "raid/platform/platform.h"

#include "raid/queue.h"

#include "logger.h"

// Test out portaudio
#include <portaudio.h>

#include <cassert>

#ifndef FORCE_TOUCHSCREEN
#define FORCE_TOUCHSCREEN 0
#endif

using namespace raid;
//----------------------------------------------
class Dummy : public Resource {
	using Resource::Resource;
	public:
		void load() override {
			debug << "Dummy asset: " << _resource_name << '\n';
			auto main_queue = QueueList::find("main");
			assert(main_queue);
			main_queue->add([this]{
				_loaded = true;
			}, _uid);
		}
};
//----------------------------------------------
class Button {
	public:
		Button(int x, int y, int width, int height, std::string texture_name) : _x(x), _y(y), _width(width), _height(height) {
			_texture = Engine::instance().get_platform()->load_texture(texture_name);
			_shader = Engine::instance().get_platform()->load_shader("test/shader/test");
		}

		template <typename Functor>
		void draw(Functor functor) {
			Engine::instance().get_platform()->draw_sprite(_x, _y, _width, _height, 0.0f, _texture, _shader, true);

			if (Engine::instance().get_platform()->is_pressed(_x, _y, _width, _height)) {
				functor();
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
			_shader = Engine::instance().get_platform()->load_shader("test/shader/test");
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
	// @todo We should make the names defines
	QueueController main_queue("main", false);
	QueueController io_queue("io", true);
	std::thread io_thread = io_queue.spawn();

	QueueList::list();

	Engine::instance().set_platform(std::make_unique<PLATFORM_IMPL>(PLATFORM_ARGS));
	auto& platform = Engine::instance().get_platform();

	auto& file_manager = Engine::instance().get_file_manager();
	file_manager.add_container("test.flx");

	auto& resource = Engine::instance().get_resource();

	//-IO-test--------------------------------------
	io_queue.add([] {
		auto& data2 = Engine::instance().get_file_manager().get_file("test/hello.txt");
		debug << data2.get_name() << '\n';
		std::stringstream stream2;
		data2.read_data(stream2);
		debug << stream2.str() << '\n';
	}, 0);
	//-Resource-test--------------------------------
	auto texture2 = resource.get<Dummy>("test/hello.txt");
	{
		// This crashes because it goes out of scope
		auto texture3 = resource.get<Dummy>("test/file");
		auto texture22 = resource.get<Dummy>("test/hello.txt");
		resource.debug_list(debug);
	}
	resource.debug_list(debug);
	auto texture33 = resource.get<Dummy>("test/file");

	resource.debug_list(debug);

	auto& timer = Engine::instance().get_timer();

	info << "Engine is starting!\n";

	//----------------------------------------------
	//-PortAudioTest--------------------------------
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		warning << "PortAudio failed to initialize!\n";
		warning << Pa_GetErrorText(err) << '\n';
	}
	//----------------------------------------------

	while(!platform->should_window_close())  {

		platform->poll_events();

		if (platform->has_context()) {
			debug << "Graphics init goes here\n";
			platform->create_window(1280, 720, "Daidalos Engine");
			platform->test_setup();

			// We use lambda's to make it easy to assign functions to buttons and to make it easy do the same action with a key
			static float x = 0;
			static float y = 0;
			/// @todo Make the speed depent on the frametime
			static float speed = 0;
			/// @note test_move_camera takes an int, but we give it a float
			auto move_left = [](){Engine::instance().get_platform()->test_move_camera(x-=speed, y);};
			auto move_right = [](){Engine::instance().get_platform()->test_move_camera(x+=speed, y);};
			auto move_up = [](){Engine::instance().get_platform()->test_move_camera(x, y-=speed);};
			auto move_down = [](){Engine::instance().get_platform()->test_move_camera(x, y+=speed);};
			Button button_left(150, 700, 100, 300, "test/textures/grass.png");
			Button button_right(350, 700, 100, 300, "test/textures/grass.png");
			Button button_up(150, 700, 300, 100, "test/textures/grass.png");
			Button button_down(150, 900, 300, 100, "test/textures/grass.png");

			auto font = platform->load_font("test/fonts/SourceSansPro-Light.ttf");
			auto font_shader = platform->load_shader("test/shader/font");

			// auto example_vorbis = resource.get<Vorbis>("test/audio/Example.ogg");
			// example_vorbis->play();

			Chunk chunk1(0,0);
			Chunk chunk2(1,0);

			while(platform->has_context() && !platform->should_window_close()) {
				timer.start();

				/// @todo This is pretty bad
				speed = 1000*timer.get_delta();

				io_queue.go();
				// @todo This should also use go()
				main_queue.process();

				platform->test_render();

				chunk1.draw();
				chunk2.draw();

				// Figure out a way to just set this on construction
				if (platform->has_touchscreen() || FORCE_TOUCHSCREEN) {
					button_left.draw(move_left);
					button_right.draw(move_right);
					button_up.draw(move_up);
					button_down.draw(move_down);
				}
				if (platform->test_check_key(65)) { move_left(); }
				if (platform->test_check_key(68)) { move_right(); }
				if (platform->test_check_key(87)) { move_up(); }
				if (platform->test_check_key(83)) { move_down(); }

				{
					std::ostringstream strs;
					strs << "Frametime: " << std::setprecision(2) << std::fixed << timer.get_delta() * 1000 << "ms"
						 << "\nLoaded assets:\n";
					resource.debug_list(strs);
					platform->draw_text(strs.str(), font, font_shader);

					/// @todo Improve the get_log_string function, maybe a vector with each line or something
					// unsigned long last_line_start = get_log_string().str().find_last_of('\n', get_log_string().str().length()-200);
					// if (last_line_start > get_log_string().str().length()) {
					// 	last_line_start = get_log_string().str().length();
					// }
					// platform->draw_text(get_log_string().str().substr(last_line_start, get_log_string().str().length()), font, font_shader);
					// platform->draw_text(get_log_string().str(), font, font_shader);

					static bool test = true;
					if (platform->test_check_key(32) && test) {
						// example_vorbis->stop();
						// example_vorbis->play();
						test = false;
					}
					if (!platform->test_check_key(32)) {
						test = true;
					}
				}

				// if (key) {
				//		move_xxx();
				// }

				platform->poll_events();
				platform->swap_buffers();

				timer.end();
			}

			debug << "Graphics cleanup code goed here\n";
			platform->terminate();
		}
	}
	
	io_queue.stop();
	io_thread.join();

	resource.debug_list(debug);
	list_valid_ids();
	debug << "Engine cleanup code goed here\n";

	//----------------------------------------------
	//-PortAudioTest--------------------------------
	err = Pa_Terminate();
	if (err != paNoError) {
		warning << "PortAudio failed to terminate!\n";
		warning << Pa_GetErrorText(err) << '\n';
	}
	//----------------------------------------------
}
