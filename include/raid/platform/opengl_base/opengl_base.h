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

			void draw_sprite(float x, float y, float width, float height, float rotation, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader) override;

			// uint load_shader();
			void test_setup() override;
			void test_render() override;
	};
	//----------------------------------------------
	class OpenGLTexture : public Texture {
		public:
			void load(std::string asset_name) override;

			GLuint get_texture_id() { return texture_id; }

		private:
			GLuint texture_id;
	};
	class OpenGLShader : public Shader {
		public:
			void load(std::string asset_name) override;

			GLuint get_program_id() { return program_id; }

		private:
			GLuint program_id;
	};
}
