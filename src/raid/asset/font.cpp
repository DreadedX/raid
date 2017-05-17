#include <vector>

#include "raid/asset/font.h"
#include "raid/engine.h"
#include "typedef.h"

#include <cassert>

void raid::Font::load_characters(int size, std::function<void(Font* font, FT_Face, byte)> process) {

	auto& font_file = Engine::instance().get_file_manager().get_file(_resource_name);
	std::vector<byte> buffer;
	font_file.read_data(buffer);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		warning << "Failed to initialize FreeType.\n";
	}

	FT_Face face;
	if (FT_New_Memory_Face(ft, buffer.data(), buffer.size(), 0, &face)) {
		warning << "Failed loading font.\n";
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	auto main_queue = QueueList::find("main");
	assert(main_queue);
	main_queue->add([=]{
		for (byte c = 0; c < 128; ++c) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				warning << "failed to load glyph.\n";
				continue;
			}

			process(this, face, c);
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		_loaded = true;
	}, _uid);

}
