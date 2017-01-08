#include "raid/platform/opengl_base/opengl_base.h"
#include "raid/engine.h"

#include "logger.h"

static const GLfloat vertices[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

std::shared_ptr<raid::Texture> raid::OpenGLBase::load_texture(std::string asset_name) {
	auto texture = Engine::instance().get_resource().get<OpenGLTexture>(asset_name);

	return std::static_pointer_cast<Texture, OpenGLTexture>(texture);
}

std::shared_ptr<raid::Shader> raid::OpenGLBase::load_shader(std::string asset_name) {
	auto shader = Engine::instance().get_resource().get<OpenGLShader>(asset_name);

	return std::static_pointer_cast<Shader, OpenGLShader>(shader);
}

GLuint vbo;
GLuint vao;
GLuint texture_id;

std::shared_ptr<raid::Shader> raid::OpenGLBase::test_setup() {
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glBindVertexArray(vao);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	return Engine::instance().get_platform()->load_shader("");
}

void raid::OpenGLBase::test_render(std::shared_ptr<raid::Shader> shader_asset) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(std::static_pointer_cast<OpenGLShader, Shader>(shader_asset)->get_program_id());
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
