#pragma once
//----------------------------------------------
#include <iostream>
#include <sstream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef ANDROID
	#include <GLES3/gl3.h>
#else
	#include <GL/glew.h>
#endif

#include "raid/engine.h"
#include "raid/asset/texture.h"
#include "raid/asset/shader.h"
#include "raid/asset/font.h"

#include "flxr/binary_helper.h"

#include "glm/glm.hpp"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class OpenGLBase : public PlatformProxy {
		public:

			std::shared_ptr<Texture> load_texture(std::string asset_name) override;
			std::shared_ptr<Shader> load_shader(std::string asset_name) override;
			std::shared_ptr<Font> load_font(std::string asset_name) override;

			void draw_sprite(float x, float y, float width, float height, float rotation, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader, bool foreground) override;
			void draw_text(std::string text, std::shared_ptr<Font> font, std::shared_ptr<Shader> shader) override;

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
	class OpenGLFont : public Font {
		using Font::Font;
		public:
			void load() override;

			auto& get_character(byte c) { return _characters[c]; }
			auto& get_characters() { return _characters; }

			struct Character {
				GLuint texture_id;
				glm::ivec2 size;
				glm::ivec2 bearing;
				long advance;
			};
		private:
			std::map<GLchar, Character> _characters;
	};
}
