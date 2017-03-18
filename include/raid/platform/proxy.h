#pragma once
//----------------------------------------------
#include <iostream>
#include <string>
#include <memory>

#include "raid/asset/texture.h"
#include "raid/asset/shader.h"
#include "raid/asset/font.h"

#include "typedef.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class PlatformProxy {
		public:
			virtual ~PlatformProxy() {}

			/// @todo Rename these functions
			virtual void create_window(int width, int height, std::string title) = 0;
			virtual void terminate() = 0;
			virtual bool should_window_close() = 0;
			virtual bool has_context() = 0;
			virtual void poll_events() = 0;
			virtual void swap_buffers() = 0;

			virtual std::string get_storage_path() = 0;

			virtual std::shared_ptr<Texture> load_texture(std::string asset_name) = 0;
			virtual std::shared_ptr<Shader> load_shader(std::string asset_name) = 0;
			virtual std::shared_ptr<Font> load_font(std::string asset_name) = 0;

			virtual void draw_sprite(float x, float y, float width, float height, float rotation, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader, bool foreground = false) = 0;
			virtual void draw_text(std::string text, std::shared_ptr<Font> font, std::shared_ptr<Shader> shader) = 0;

			virtual bool is_pressed(int x, int y, int width, int height) = 0;

			virtual bool has_touchscreen() = 0;

			/// @todo Return a shader asset derived from a platform independent shader asset
			// virtual uint load_shader() = 0;
			virtual void test_setup() = 0;
			virtual void test_render() = 0;
			virtual void test_move_camera(float x, float y) = 0;

			virtual bool test_check_key(int) = 0;
	};
	//----------------------------------------------
	class DummyPlatform : public PlatformProxy {
		public:
			virtual ~DummyPlatform() {}

			virtual void create_window(int, int, std::string) {
				warning << "Using dummy platform\n";
			}
			virtual void terminate() {}
			virtual bool should_window_close() { return false; }
			virtual bool has_context() { return true; }
			virtual void poll_events() {}
			virtual void swap_buffers() {}

			virtual std::string get_storage_path() { return "."; };

			virtual std::shared_ptr<Texture> load_texture(std::string) { return nullptr; }
			virtual std::shared_ptr<Shader> load_shader(std::string) { return nullptr; }
			virtual std::shared_ptr<Font> load_font(std::string) { return nullptr; }

			virtual void draw_sprite(float, float, float, float, float, std::shared_ptr<Texture>, std::shared_ptr<Shader>, bool) { return; }
			virtual void draw_text(std::string, std::shared_ptr<Font>, std::shared_ptr<Shader>) {return; }

			virtual bool is_pressed(int, int, int, int) {return false; }

			virtual bool has_touchscreen() { return false; }

			/// @todo Return a shader asset derived from a platform independent shader asset
			// virtual uint load_shader() = 0;
			virtual void test_setup() {}
			virtual void test_render() {}
			virtual void test_move_camera(float, float) { return; }

			virtual bool test_check_key(int) { return false; }
	};
}
