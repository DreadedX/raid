#include <vector>
#include "stdint.h"
void glSpecializeShaderARB(uint32_t, const char*, uint32_t, const uint32_t*, const uint32_t*) __attribute__((weak));

#include <spirv_glsl.hpp>

#include "raid/platform/opengl_base/opengl_base.h"
#include "raid/engine.h"

#include "logger.h"

#ifndef GL_SHADER_BINARY_FORMAT_SPIR_V_ARB
#define GL_SHADER_BINARY_FORMAT_SPIR_V_ARB 0
#endif

static const GLfloat vertices[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

std::string get_shader_string(std::vector<byte> _binary) {
	
	// Figure out something better
	std::vector<uint32_t> binary;
	for (uint i = 0; i < _binary.size(); i+=4) {
		uint32_t temp = 0;
		temp += _binary[i+0] << 0*8;
		temp += _binary[i+1] << 1*8;
		temp += _binary[i+2] << 2*8;
		temp += _binary[i+3] << 3*8;

		binary.push_back(temp);
	}

	spirv_cross::CompilerGLSL glsl(std::move(binary));

	// The SPIR-V is now parsed, and we can perform reflection on it.
	spirv_cross::ShaderResources resources = glsl.get_shader_resources();

	// Get all sampled images in the shader.
	for (auto &resource : resources.sampled_images)
	{
		unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

		// Modify the decoration to prepare it for GLSL.
		glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

		// Some arbitrary remapping if we want.
		glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
	}

	// Set some options.
	spirv_cross::CompilerGLSL::Options options;
	#ifdef ANDROID
		options.version = 300;
		options.es = true;
	#else
		options.version = 330;
		options.es = false;
	#endif
	options.vertex.fixup_clipspace = false;
	glsl.set_options(options);

	// Compile to GLSL, ready to give to GL driver.
	std::string source = glsl.compile();

	return source;
}

void raid::OpenGLShader::load(std::string) {
	auto& file_manager = Engine::instance().get_file_manager();
	auto& vertex_file = file_manager.get_file("test/shader/test.vert");
	auto& fragment_file = file_manager.get_file("test/shader/test.frag");

	std::vector<byte> vertex_binary;
	vertex_file.read_data(vertex_binary);
	std::vector<byte> fragment_binary;
	fragment_file.read_data(fragment_binary);

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	if (glSpecializeShaderARB) {
		glShaderBinary(1, &VertexShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, vertex_binary.data(), vertex_binary.size());
		glSpecializeShaderARB(VertexShaderID, "main", 0, nullptr, nullptr);
	} else {
		std::string vertex_source_string = get_shader_string(vertex_binary);
		const char* vertex_source = vertex_source_string.c_str();
		debug << vertex_source << '\n';
		glShaderSource(VertexShaderID, 1, &vertex_source, NULL);
	}
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		warning << &VertexShaderErrorMessage[0] << '\n';
	}

	// Compile Fragment Shader
	if (glSpecializeShaderARB) {
		glShaderBinary(1, &FragmentShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, fragment_binary.data(), fragment_binary.size());
		glSpecializeShaderARB(FragmentShaderID, "main", 0, nullptr, nullptr);
	} else {
		std::string fragment_source_string = get_shader_string(fragment_binary);
		const char* fragment_source = fragment_source_string.c_str();
		debug << fragment_source << '\n';
		glShaderSource(FragmentShaderID, 1, &fragment_source, NULL);
	}
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		warning << &FragmentShaderErrorMessage[0] << '\n';
	}

	// Link the program
	debug << "Linking program\n";
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		warning << &ProgramErrorMessage[0] << '\n';
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	program_id = ProgramID;
}

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
