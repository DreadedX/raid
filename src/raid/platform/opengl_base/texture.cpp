#include "raid/platform/opengl_base/opengl_base.h"

void raid::OpenGLTexture::load(std::string asset_name) {
	debug << "Loading asset: " << asset_name << '\n';
	auto& file_manager = Engine::instance().get_file_manager();
	auto texture_meta_data = file_manager.get_file("test/textures/test.png");
	std::stringstream data_stream;
	texture_meta_data.read_data(data_stream);
	for (unsigned int i = 0; i < data_stream.str().length(); ++i) {
		byte dat;
		flxr::read(data_stream, dat);
		debug << std::hex << (int)dat << ' ';
	}
	debug << '\n';
}
