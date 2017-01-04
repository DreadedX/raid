#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <shaderc/shaderc.hpp>
#include "file_includer.h"
#include <spirv_glsl.hpp>

#include "flxr/binary_helper.h"
#include "flexy/plugin.h"

const bool OPTIMIZE = false;

class Shader : public Plugin {
	public:
		virtual std::shared_ptr<std::iostream> process(std::string file_path) override {
			std::string source;
			auto file_stream = open_file(file_path);
			flxr::read(*file_stream, source);

			shaderc::Compiler compiler;
			shaderc::CompileOptions options;

			// Like -DMY_DEFINE=1
			// Get this from the config
			options.AddMacroDefinition("MY_DEFINE", "1");
			if (OPTIMIZE) options.SetOptimizationLevel(shaderc_optimization_level_size);
			// LEAKLEAKLEAK
			auto finder = new shaderc_util::FileFinder;
			// Get this from the config
			finder->search_path() = {"../../../assets/include"};
			options.SetIncluder(std::make_unique<glslc::FileIncluder>(finder));

			shaderc::SpvCompilationResult module =
				compiler.CompileGlslToSpv(source, shaderc_glsl_infer_from_source, file_path.c_str(), options);

			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				warning << module.GetErrorMessage() << '\n';
				exit(-1);
				return nullptr;
			}

			std::shared_ptr<std::iostream> output_stream = std::make_shared<std::stringstream>();
			std::vector<uint32_t> binary = {module.cbegin(), module.cend()};
			flxr::write(*output_stream, binary);

			{
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
				// options.version = 310;
				// options.es = true;
				options.version = 330;
				options.es = false;
				options.vertex.fixup_clipspace = false;
				glsl.set_options(options);

				// Compile to GLSL, ready to give to GL driver.
				std::string source = glsl.compile();

				debug << source << '\n';
			}

			return output_stream;
		}
};
share_plugin(Shader)
