#include <vector>
#include "stdint.h"
void glSpecializeShaderARB(uint32_t, const char*, uint32_t, const uint32_t*, const uint32_t*) __attribute__((weak));

// Order is important here
#include <spirv_glsl.hpp>

#include "raid/platform/opengl_base/opengl_base.h"

#ifndef GL_SHADER_BINARY_FORMAT_SPIR_V_ARB
#define GL_SHADER_BINARY_FORMAT_SPIR_V_ARB 0
#endif

// This uses spriv-cross to generate glsl from a spir-v binary
std::string get_shader_string(std::vector<byte> _binary) {
	
	// Figure out something better
	std::vector<uint32_t> binary;
	for (uint32_t i = 0; i < _binary.size(); i+=4) {
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

	/// @note This generates a shader that does not work on opengl es 3.0, but it is really usefull!
	// Get all sampled images in the shader.
	for (auto &resource : resources.sampled_images)
	{
		// unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		// unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		// printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);
        //
		// // Modify the decoration to prepare it for GLSL.
		// glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        //
		// // Some arbitrary remapping if we want.
		// glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
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

const bool BINARY = true;
void raid::OpenGLShader::load() {

	auto& file_manager = Engine::instance().get_file_manager();
	auto& vertex_file = file_manager.get_file(_resource_name + ".vert");
	auto& fragment_file = file_manager.get_file(_resource_name + ".frag");

	std::vector<byte> vertex_binary;
	vertex_file.read_data(vertex_binary);
	std::vector<byte> fragment_binary;
	fragment_file.read_data(fragment_binary);

	auto main_queue = QueueList::find("main");
	main_queue->add([=] {

			// Create the shaders
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			GLint Result = GL_FALSE;
			int InfoLogLength;

			// Compile Vertex Shader
			if (glSpecializeShaderARB && BINARY) {
				glShaderBinary(1, &VertexShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, vertex_binary.data(), vertex_binary.size());
				glSpecializeShaderARB(VertexShaderID, "main", 0, nullptr, nullptr);
			} else {
				std::string vertex_source_string = get_shader_string(vertex_binary);
				const char* vertex_source = vertex_source_string.c_str();
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
			if (glSpecializeShaderARB && BINARY) {
				glShaderBinary(1, &FragmentShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, fragment_binary.data(), fragment_binary.size());
				glSpecializeShaderARB(FragmentShaderID, "main", 0, nullptr, nullptr);
			} else {
				std::string fragment_source_string = get_shader_string(fragment_binary);
				const char* fragment_source = fragment_source_string.c_str();
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

			_loaded = true;
	}, _uid);
}
