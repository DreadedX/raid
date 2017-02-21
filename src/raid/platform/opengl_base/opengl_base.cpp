#include "raid/platform/opengl_base/opengl_base.h"
#include "raid/engine.h"

#include "logger.h"

/// @todo Make this part of the actual project
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static const GLfloat vertices[] = {
	// Pos      // Tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

std::shared_ptr<raid::Texture> raid::OpenGLBase::load_texture(std::string asset_name) {
	auto texture = Engine::instance().get_resource().get<OpenGLTexture>(asset_name);

	return std::static_pointer_cast<Texture, OpenGLTexture>(texture);
}

std::shared_ptr<raid::Shader> raid::OpenGLBase::load_shader(std::string asset_name) {
	auto shader = Engine::instance().get_resource().get<OpenGLShader>(asset_name);

	return std::static_pointer_cast<Shader, OpenGLShader>(shader);
}

std::shared_ptr<raid::Font> raid::OpenGLBase::load_font(std::string asset_name) {
	auto font = Engine::instance().get_resource().get<OpenGLFont>(asset_name);

	return std::static_pointer_cast<Font, OpenGLFont>(font);
}

// Store this somewhere (A model or something...)
static GLuint vao;
// Store this in the shader
static GLuint ubo;

void raid::OpenGLBase::draw_sprite(float x, float y, float width, float height, float rotation, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader, bool foreground) {
	GLuint shader_id = std::static_pointer_cast<OpenGLShader, Shader>(shader)->get_program_id();

	glUseProgram(shader_id);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x, y, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));

	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	glm::mat4 projection;
	if(foreground) {
		projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
	} else {
		projection = glm::ortho(0.0f+camera_x, 1920.0f+camera_x, 1080.0f+camera_y, 0.0f+camera_y, -1.0f, 1.0f);
	}

	GLuint block_index = glGetUniformBlockIndex(shader_id, "transformations");
	GLint block_size;

	glGetActiveUniformBlockiv(shader_id, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

	// !!! WARNING: DO NOT USE RAW POINTERS !!!
	GLubyte *block_buffer = (GLubyte *) malloc(block_size);

	const GLchar *names[] = {"transformations.projection", "transformations.model"};
	GLuint indices[2];
	glGetUniformIndices(shader_id, 2, names, indices);

	GLint offset[2];
	glGetActiveUniformsiv(shader_id, 2, indices, GL_UNIFORM_OFFSET, offset);

	memcpy(block_buffer + offset[0], glm::value_ptr(projection), sizeof(glm::mat4));
	memcpy(block_buffer + offset[1], glm::value_ptr(model), sizeof(glm::mat4));

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, block_size, block_buffer, GL_DYNAMIC_DRAW);

	// @todo Is this valid?
	free(block_buffer);

	glBindBufferBase(GL_UNIFORM_BUFFER, block_index, ubo);
	glUniform1i(glGetUniformLocation(shader_id, "image"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, std::static_pointer_cast<OpenGLTexture, Texture>(texture)->get_texture_id());

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0);
}

void raid::OpenGLBase::draw_text(std::string text, std::shared_ptr<Font> font, std::shared_ptr<Shader> shader) {
	int x = 50;
	int y = 50 + 25;

	float scale = 1.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	GLuint shader_id = std::static_pointer_cast<OpenGLShader, Shader>(shader)->get_program_id();

	glUseProgram(shader_id);
	
	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);

	GLuint vao_text, vbo_text;
	{
		glGenVertexArrays(1, &vao_text);
		glGenBuffers(1, &vbo_text);
		glBindVertexArray(vao_text);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_text);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);      
	}

	/// @todo Refactor this
	GLuint block_index1 = glGetUniformBlockIndex(shader_id, "transformations");
	{
		GLint block_size;

		glGetActiveUniformBlockiv(shader_id, block_index1, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

		// !!! WARNING: DO NOT USE RAW POINTERS !!!
		GLubyte *block_buffer = (GLubyte *) malloc(block_size);

		const GLchar *names[] = {"transformations.projection"};
		GLuint indices[1];
		glGetUniformIndices(shader_id, 1, names, indices);

		GLint offset[1];
		glGetActiveUniformsiv(shader_id, 1, indices, GL_UNIFORM_OFFSET, offset);

		memcpy(block_buffer + offset[0], glm::value_ptr(projection), sizeof(glm::mat4));

		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, block_size, block_buffer, GL_DYNAMIC_DRAW);

		// @todo Is this valid?
		free(block_buffer);
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, block_index1, ubo);
	glUniform1i(glGetUniformLocation(shader_id, "text"), 0);

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vao_text);
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c) {
		auto ch = std::static_pointer_cast<OpenGLFont, Font>(font)->get_character(*c);

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		/// @todo The image is flipped without the -, but this also moves the text
		GLfloat h = -ch.size.y * scale;

		GLfloat vertices_text[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },            
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }           
		};

		glBindBuffer(GL_ARRAY_BUFFER, vbo_text);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_text), vertices_text);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle

		x += (ch.advance >> 6) * scale;
	}
	glBindVertexArray(0);
}

void raid::OpenGLBase::test_setup() {
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	GLuint vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &ubo);
}

void raid::OpenGLBase::test_render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
