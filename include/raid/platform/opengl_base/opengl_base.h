#pragma once
//----------------------------------------------
#include <iostream>
#include <sstream>

#ifdef ANDROID
	#include <GLES3/gl3.h>
#else
	#include <GL/glew.h>
#endif

#include "raid/engine.h"
#include "raid/asset/texture.h"
#include "raid/asset/shader.h"

#include "flxr/binary_helper.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class OpenGLBase : public PlatformProxy {
		public:
			std::shared_ptr<Texture> load_texture(std::string asset_name) override;
			std::shared_ptr<Shader> load_shader(std::string asset_name) override;

			void draw_sprite(float x, float y, float width, float height, float rotation, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader, bool foreground) override;

			// uint load_shader();
			void test_setup() override;
			void test_render() override;
			void test_move_camera(float x, float y) override { camera_x = x; camera_y = y; }

		private:
			float camera_x = 0;
			float camera_y = 0;
	};
	//----------------------------------------------
	class OpenGLTexture : public Texture {
		using Texture::Texture;
		public:
			void load() override;

			GLuint get_texture_id() { return texture_id; }

		private:
			GLuint texture_id;
	};
	class OpenGLShader : public Shader {
		using Shader::Shader;
		public:
			void load() override;

			GLuint get_program_id() { return program_id; }

		private:
			GLuint program_id;
	};
}
