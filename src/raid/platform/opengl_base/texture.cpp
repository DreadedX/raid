#include "raid/platform/opengl_base/opengl_base.h"

void raid::OpenGLTexture::load() {
	debug << "Loading asset: " << _resource_name << '\n';
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/** @todo This needs all kinds of checking */
//	float amount = 4.0f;
//	debug << "Anisotropic: " << amount << '\n';
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);

	if (_resource_name != "debug") {
		auto& file_manager = Engine::instance().get_file_manager();
		auto texture_meta_data = file_manager.get_file(_resource_name);
		std::vector<byte> texture_data;
		texture_meta_data.read_data(texture_data);

		int width = 0;
		int height = 0;

		uint32 offset = 0;
		for (uint32 i = 0; i < sizeof(int); ++i) {
			width += texture_data[i] << (i*8);
		}
		offset += sizeof(int);

		for (uint32 i = 0; i < sizeof(int); ++i) {
			height += texture_data[i + offset] << (i*8);
		}
		offset += sizeof(int);

		byte bytes_per_pixel = texture_data[offset];
		offset += sizeof(byte);

		/** @todo Check if color space is linear or sRGB */
		if (bytes_per_pixel == 0x04) {

//			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data() + offset);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data() + offset);
		} else if (bytes_per_pixel == 0x03) {

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data.data() + offset);
		} else {

			warning << '\"' << _resource_name << "contains an invalid amount of bytes per pixel (" << bytes_per_pixel << ")\n";
		}

		debug << "IMAGE: " << width << ", " << height << ", " << (int)bytes_per_pixel << ": " << _resource_name << '\n';
	} else {

		debug << "Generating empty texture\n";

		byte texture_data[] =  {
			0xFF, 0xFF, 0xFF,  0x00, 0x00, 0x00,
			0x00, 0x00, 0x00,  0xFF, 0xFF, 0xFF
		};

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	}
}
