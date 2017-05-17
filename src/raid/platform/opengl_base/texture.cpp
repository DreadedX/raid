#include "raid/platform/opengl_base/opengl_base.h"

#include "raid/queue.h"

#include <cassert>

void raid::OpenGLTexture::load() {
	auto& file_manager = Engine::instance().get_file_manager();
	std::vector<byte> texture_data;

	if (_resource_name != "debug") {
		auto texture_meta_data = file_manager.get_file(_resource_name);
		texture_meta_data.read_data(texture_data);
	} else {
		// Width: 2
		texture_data.push_back(0x02); texture_data.push_back(0x00); texture_data.push_back(0x00); texture_data.push_back(0x00);
		// Height: 2
		texture_data.push_back(0x02); texture_data.push_back(0x00); texture_data.push_back(0x00); texture_data.push_back(0x00);
		// BytesPerPixel: 3
		texture_data.push_back(0x03);
		texture_data.push_back(0xFF); texture_data.push_back(0xFF); texture_data.push_back(0xFF);
		texture_data.push_back(0x00); texture_data.push_back(0x00); texture_data.push_back(0x00);
		texture_data.push_back(0x00); texture_data.push_back(0x00); texture_data.push_back(0x00);
		texture_data.push_back(0xFF); texture_data.push_back(0xFF); texture_data.push_back(0xFF);
	}

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

	debug << "IMAGE: " << width << ", " << height << ", " << (int)bytes_per_pixel << ": " << _resource_name << '\n';

	auto main_queue = QueueList::find("main");
	assert(main_queue);
	main_queue->add([=] {
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

			/** @todo Check if color space is linear or sRGB */
			if (bytes_per_pixel == 0x04) {

				//			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data() + offset);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data() + offset);
			} else if (bytes_per_pixel == 0x03) {

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data.data() + offset);
			} else {

				warning << '\"' << _resource_name << "contains an invalid amount of bytes per pixel (" << bytes_per_pixel << ")\n";
			}

			_loaded = true;
	}, _uid);
}
