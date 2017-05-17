#pragma once

#include <mutex>
#include <vector>
#include <portaudio.h>

#define STB_VORBIS_HEADER_ONLY
#include "raid/stb_vorbis.h"

#include "raid/resource.h"

#include "typedef.h"

namespace raid {
	// This really should inherit from a base audio class that way other audio types can also be used
	class Vorbis : public Resource {
		using Resource::Resource;
		public:
			~Vorbis();

			void load() override;
			void play();
			void stop();

			void set_volume(float volume_level_db);

			struct PlaybackData {
				stb_vorbis* v = nullptr;
				float volume_multiplier = 1.0f;
				std::mutex mtx;
			};

		private:
			PaStream* _stream = nullptr;
			std::vector<byte> _data;
			PlaybackData _playback_data;
	};
}
