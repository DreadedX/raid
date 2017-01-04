#pragma once

#include "raid/resource.h"

#include "logger.h"

namespace raid {
	class Shader : public Resource {
		public:
			Shader() {
				debug << "I am a shader\n";
			}
	};
}
