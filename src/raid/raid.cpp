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

#include "flxr/spec.h"
#include "flxr/binary_helper.h"
#include "flxr/exceptions.h"

#include "logger.h"

#include "raid/platform/platform.h"

// Test out portaudio
#include "portaudio.h"

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
typedef struct {
	float left_phase;
	float right_phase;
} PaTestData;

static int test_audio_callback(const void*, void* output_buffer,
                               unsigned long frames_per_second,
                               const PaStreamCallbackTimeInfo*,
                               PaStreamCallbackFlags,
                               void* user_data) {
	PaTestData* data = (PaTestData*)user_data;
	float* out = (float*)output_buffer;

	for(unsigned int i=0; i < frames_per_second; ++i) {
		*out++ = data->left_phase;
		*out++ = data->right_phase;

		data->left_phase += 0.01f;
		if (data->left_phase >= 1.0f) {
			data->left_phase -= 2.0f;
		}
		data->right_phase += 0.03f;
		if (data->right_phase >= 1.0f) {
			data->right_phase -= 2.0f;
		}
	}
	return 0;
}

#define SAMPLE_RATE (44100)
static PaTestData data;

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
	resource.debug_list(debug);
	auto texture33 = resource.get<Dummy>("test/file");
	
	resource.debug_list(debug);

	auto& timer = Engine::instance().get_timer();

	//----------------------------------------------
	//-PortAudioTest--------------------------------
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		warning << "PortAudio failed to initialize!";
		warning << Pa_GetErrorText(err);
	}

	PaStream* stream;
	err = Pa_OpenDefaultStream( &stream, 0, 2, paFloat32, SAMPLE_RATE, 256, test_audio_callback, &data);
	if (err != paNoError) {
		warning << "PortAudio error!";
		warning << Pa_GetErrorText(err);
	}

	err = Pa_StartStream(stream);
	if (err != paNoError) {
		warning << "PortAudio error!";
		warning << Pa_GetErrorText(err);
	}
	//----------------------------------------------

	while(!platform->should_window_close())  {

		platform->poll_events();

		if (platform->has_context()) {
			debug << "Graphics init goes here\n";
			// LOGI("Graphics init goes here");
			platform->create_window(1280, 720, "Daidalos Engine");
			platform->test_setup();

			Chunk chunk1(0,0);
			Chunk chunk2(1,0);

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

			while(platform->has_context() && !platform->should_window_close()) {
				timer.start();

				/// @todo This is pretty bad
				speed = 1000*timer.get_delta();

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
						debug << "Just a test\n";
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
			// LOGI("Graphics cleanup code goed here");
			platform->terminate();
		}
	}
	resource.debug_list(debug);
	// LOGI("Engine cleanup code goed here");
	debug << "Engine cleanup code goed here\n";

	//----------------------------------------------
	//-PortAudioTest--------------------------------
	err = Pa_StopStream(stream);
	if (err != paNoError) {
		warning << "PortAudio error!";
		warning << Pa_GetErrorText(err);
	}
	err = Pa_Terminate();
	if (err != paNoError) {
		warning << "PortAudio failed to terminate!";
		warning << Pa_GetErrorText(err);
	}
	//----------------------------------------------
}
