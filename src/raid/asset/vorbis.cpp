#include "raid/stb_vorbis.h"

#include "raid/engine.h"
#include "raid/asset/vorbis.h"

/// @todo We cannot start the same audio file twice...
int vorbis_audio_callback(const void*, void* output_buffer, unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* user_data) {
	static_cast<raid::Vorbis::PlaybackData*>(user_data)->mtx.lock();
		stb_vorbis *v = static_cast<raid::Vorbis::PlaybackData*>(user_data)->v;
		int num_c = v->channels;
		float volume_multiplier = static_cast<raid::Vorbis::PlaybackData*>(user_data)->volume_multiplier;

		int n = stb_vorbis_get_samples_float_interleaved(v, num_c, static_cast<float*>(output_buffer), static_cast<int>(num_c*frames_per_buffer));

		// Volume calculation
		for (unsigned long i = 0; i < num_c*frames_per_buffer; ++i) {
			static_cast<float*>(output_buffer)[i] *= volume_multiplier; // Maybe also add a master volume multiplier here
		}
	static_cast<raid::Vorbis::PlaybackData*>(user_data)->mtx.unlock();
	
	debug << n << "/" << num_c*frames_per_buffer << '\n';

	if (n < static_cast<int>(num_c*frames_per_buffer)) {
		debug << "Finished audio stream\n";
		return paComplete;
	}
	return paContinue;
}

void raid::Vorbis::load() {
	int err = VORBIS__no_error;
	// _playback_data.v = stb_vorbis_open_filename(_resource_name.c_str(), &err, nullptr);
	auto metadata = Engine::instance().get_file_manager().get_file(_resource_name);
	metadata.read_data(_data);
	_playback_data.v = stb_vorbis_open_memory(_data.data(), _data.size(), &err, nullptr);

	if (!_playback_data.v) {
		warning << "Failed to open file: " << _resource_name << '\n';
	}

	// Need more data is not a real error
	if (err != VORBIS__no_error && err != VORBIS_need_more_data) {
		warning << "stb_vorbis encountered an error: " << err << '\n';
	}

	// We need to check if v->channels is not bigger than maxOutputChannels and we need to pass the final channel count to the callback
	err = Pa_OpenDefaultStream(&_stream, 0, _playback_data.v->channels, paFloat32, _playback_data.v->sample_rate, 0, vorbis_audio_callback, &_playback_data);
	if (err != paNoError) {
		warning << "PortAudio error!\n";
		warning << Pa_GetErrorText(err) << '\n';
	}

}

/// @todo This should include a check to see if the audio has already stopped playing
void raid::Vorbis::play() {
	if (_playback_data.v) {
		_playback_data.mtx.lock();
			stb_vorbis_seek_start(_playback_data.v);
		_playback_data.mtx.unlock();

		int error = stb_vorbis_get_error(_playback_data.v);
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
	if (_playback_data.v) {
		int err = Pa_StopStream(_stream);
		if (err != paNoError) {
			warning << "PortAudio error!\n";
			warning << Pa_GetErrorText(err) << '\n';
		}
	} else {
		warning << "Vorbis file is not open: " << _resource_name << '\n';
	}
}

#define VOLUME_REFERENCE 1.0f

void raid::Vorbis::set_volume(float volume_level_db) {
	debug << "Volume was: " << _playback_data.volume_multiplier << '\n';
	_playback_data.mtx.lock();
		_playback_data.volume_multiplier = (VOLUME_REFERENCE * pow(10, (volume_level_db / 20.0f)));
	_playback_data.mtx.unlock();
	debug << "Volume set to: " << _playback_data.volume_multiplier << '\n';
}

raid::Vorbis::~Vorbis() {
	stop();
}
