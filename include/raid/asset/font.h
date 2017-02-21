#pragma once
#include <functional>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "raid/resource.h"
#include "typedef.h"

namespace raid {

	class Font : public Resource {
		using Resource::Resource;
		protected:
			void load_characters(int size, std::function<void(Font*, FT_Face, byte c)> process);
	};
}
