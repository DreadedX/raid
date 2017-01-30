#pragma once
//----------------------------------------------
#include <iostream>
#include <string>
#include <memory>

#include "raid/asset/texture.h"
#include "raid/asset/shader.h"

#include "typedef.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class PlatformProxy {
		public:
			virtual ~PlatformProxy() {}

			virtual void create_window(int width, int height, std::string title) = 0;
			virtual void terminate() = 0;
			virtual bool should_window_close() = 0;
			virtual bool has_context() = 0;
			virtual void poll_events() = 0;
			virtual void swap_buffers() = 0;

			virtual std::string get_storage_path() = 0;

			virtual std::shared_ptr<Texture> load_texture(std::string asset_name) = 0;
			virtual std::shared_ptr<Shader> load_shader(std::string asset_name) = 0;

			virtual void draw_sprite(float x, float y, float width, float height, float rotation, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader) = 0;

			/// @todo Return a shader asset derived from a platform independent shader asset
			// virtual uint load_shader() = 0;
			virtual void test_setup() = 0;
			virtual void test_render() = 0;
	};
	//----------------------------------------------
	class DummyPlatform : public PlatformProxy {
		public:
				virtual ~DummyPlatform() {}

				virtual void create_window(int, int, std::string) {}
				virtual void terminate() {}
				virtual bool should_window_close() { return false; }
				virtual bool has_context() { return true; }
				virtual void poll_events() {}
				virtual void swap_buffers() {}

				virtual std::string get_storage_path() { return "."; };

				virtual std::shared_ptr<Texture> load_texture(std::string) { return nullptr; }
				virtual std::shared_ptr<Shader> load_shader(std::string) { return nullptr; }

				/// @todo Return a shader asset derived from a platform independent shader asset
				// virtual uint load_shader() = 0;
				virtual void test_setup() {}
				virtual void test_render(std::shared_ptr<Shader>, std::shared_ptr<Texture>) {}
	};
}
