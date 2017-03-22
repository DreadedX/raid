#include "raid/asset/vorbis.h"
#include "raid/stb_vorbis.h"

stb_vorbis *_vorbis = nullptr;

/// @todo We cannot start the same audio file twice...
int vorbis_audio_callback(const void*, void* output_buffer, unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* user_data) {
	stb_vorbis *v = static_cast<stb_vorbis*>(user_data);
	int num_c = v->channels;
	int n = stb_vorbis_get_samples_float_interleaved(v, num_c, static_cast<float*>(output_buffer), static_cast<int>(num_c*frames_per_buffer));

	// for (int i = n; i < num_c*frames_per_buffer; ++i) {
	// 	static_cast<float*>(output_buffer)[i] = 0.0f;
	// }
	
	debug << n << "/" << num_c*frames_per_buffer << '\n';

	if (n < num_c*frames_per_buffer) {
		debug << "Finished audio stream\n";
		return paComplete;
	}
	return paContinue;
}

void raid::Vorbis::load() {
	int err = VORBIS__no_error;
	_vorbis = stb_vorbis_open_filename(_resource_name.c_str(), &err, nullptr);

	if (!_vorbis) {
		warning << "Failed to open file: " << _resource_name << '\n';
	}

	// Need more data is not a real error
	if (err != VORBIS__no_error && err != VORBIS_need_more_data) {
		warning << "stb_vorbis encountered an error: " << err << '\n';
	}

	// We need to check if v->channels is not bigger than maxOutputChannels and we need to pass the final channel count to the callback
	err = Pa_OpenDefaultStream(&_stream, 0, _vorbis->channels, paFloat32, _vorbis->sample_rate, 0, vorbis_audio_callback, _vorbis);
	if (err != paNoError) {
		warning << "PortAudio error!\n";
		warning << Pa_GetErrorText(err) << '\n';
	}

}

/// @todo This should include a check to see if the audio has already stopped playing
void raid::Vorbis::play() {
	if (_vorbis) {
		stb_vorbis_seek_start(_vorbis);

		int error = stb_vorbis_get_error(_vorbis);
		if (error) {
			warning << "stb_vorbis encountered an error: " << error << '\n';
		}

		int err = Pa_StartStream(_stream);
		if (err != paNoError) {
			warning << "PortAudio error!\n";
			warning << Pa_GetErrorText(err) << '\n';
		}
	} else {
		warning << "Vorbis file is not open: " << _resource_name << '\n';
	}
}

void raid::Vorbis::stop() {
	if (_vorbis) {
		int err = Pa_StopStream(_stream);
		if (err != paNoError) {
			warning << "PortAudio error!\n";
			warning << Pa_GetErrorText(err) << '\n';
		}
	} else {
		warning << "Vorbis file is not open: " << _resource_name << '\n';
	}
}

raid::Vorbis::~Vorbis() {
	stop();
}
