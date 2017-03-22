#pragma once

#include <portaudio.h>

#include "raid/resource.h"

namespace raid {
	class Vorbis : public Resource {
		using Resource::Resource;

		public:
			~Vorbis();

			void load() override;
			void play();
			void stop();

		private:
			PaStream* _stream = nullptr;
	};
}
