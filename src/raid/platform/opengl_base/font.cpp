#include "raid/platform/opengl_base/opengl_base.h"

#include "logger.h"

#include <cassert>

void process(raid::Font* font, FT_Face face, byte c) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	raid::OpenGLFont::Character character = {
		texture,
		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		face->glyph->advance.x
	};
	dynamic_cast<raid::OpenGLFont*>(font)->get_characters().insert(std::pair<GLchar, raid::OpenGLFont::Character>(c, character));
}

void raid::OpenGLFont::load() {
	auto main_queue = QueueList::find("main");
	assert(main_queue);
	main_queue->add([this]{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   
	}, _uid);
	load_characters(48, process);
}
