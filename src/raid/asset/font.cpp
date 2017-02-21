#include <vector>

#include "raid/asset/font.h"
#include "raid/engine.h"
#include "typedef.h"

void raid::Font::load_characters(int size, std::function<void(Font* font, FT_Face, byte)> process) {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		warning << "Failed to initialize FreeType.\n";
	}

	FT_Face face;
	auto& font_file = Engine::instance().get_file_manager().get_file(_resource_name);
	std::vector<byte> buffer;
	font_file.read_data(buffer);
	if (FT_New_Memory_Face(ft, buffer.data(), buffer.size(), 0, &face)) {
		warning << "Failed loading font.\n";
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	for (byte c = 0; c < 128; ++c) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			warning << "failed to load glyph.\n";
			continue;
		}

		process(this, face, c);

	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
