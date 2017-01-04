#pragma once

#include "raid/resource.h"

#include "logger.h"

namespace raid {
	class Texture : public Resource {
		public:
			Texture() {
				debug << "I am a texture\n";
			}
	};
}
