#pragma once

#include <string>
#include <SDL_mixer.h>
#include <cassert>


#define _CHECK_CHANNEL_(channel) \
assert(channel >= -1 && channel < static_cast<int>(channels_), "Invalid audio channel value");

class SoundEffect {
private:
	Mix_Chunk* chunk_;
	static int channels_;

	inline static void checkChannel(int channel) {
		assert(channel >= -1 && channel < static_cast<int>(channels_), "Invalid audio channel value");
	}


public:
	// Constructor
	SoundEffect(const std::string& fileName) {
		channels_ = 0;
		chunk_ = Mix_LoadWAV(fileName.c_str());
		if (chunk_ == nullptr) {
			throw "Couldn't load effect: " + fileName;
		}
	}

	// To avoid copies 
	SoundEffect& operator=(SoundEffect& other) = delete;
	SoundEffect(const SoundEffect&) = delete;

	// Can be moved
	SoundEffect(SoundEffect&& other) noexcept {
		chunk_ = other.chunk_;
		other.chunk_ = nullptr;
	}
	SoundEffect& operator=(SoundEffect&& other) noexcept {
		this->~SoundEffect();
		chunk_ = other.chunk_;
		other.chunk_ = nullptr;
		return *this;
	}
	
	virtual ~SoundEffect() {
		if (chunk_ != nullptr)
			Mix_FreeChunk(chunk_);
	}


	// To play the sound effect an amount of times (-1 for infinite times) in a specific channel 
	inline int play(int loops = 0, int channel = -1) const {
		_CHECK_CHANNEL_(channel);
		assert(loops >= -1, "Audio set to loop for invalid amount of times");
		return Mix_PlayChannel(channel, chunk_, loops);
	}

	// To change the volume (ranges from 0 to 128 both included)
	inline int setVolume(int volume) {
		assert(volume >= 0 && volume <= 128, "Invalid volume value");
		return Mix_VolumeChunk(chunk_, volume);
	}

	// Pause/resume/stop a specific channel
	inline static void pauseChannel(int channel = -1) {
		_CHECK_CHANNEL_(channel);
		Mix_Pause(channel);
	}
	inline static void resumeChannel(int channel = -1) {
		_CHECK_CHANNEL_(channel);
		Mix_Resume(channel);
	}
	inline static void haltChannel(int channel = -1) {
		_CHECK_CHANNEL_(channel);
		Mix_HaltChannel(channel);
	}


	// To change the channel volume (ranges from 0 to 128 both included)
	inline static int setChannelVolume(int volume, int channel = -1) {
		_CHECK_CHANNEL_(channel);
		assert(volume >= 0 && volume <= 128, "Invalid volume value");
		return Mix_Volume(channel, volume);
	}

	// To change the number of channels
	inline static int setNumberofChannels(int n) {
		assert(n > 0, "Invalid amount of channels");
		return channels_ = Mix_AllocateChannels(n);
	}


};

