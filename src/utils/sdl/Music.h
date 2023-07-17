#pragma once

#include <string>
#include <SDL_mixer.h>

class Music {
private:
	Mix_Music* music_;

public:
	// Constructor
	Music(const std::string& fileName) {
		music_ = Mix_LoadMUS(fileName.c_str());
		assert(music_ != nullptr);
	}

	// To avoid copies 
	Music& operator=(Music& other) = delete;
	Music(const Music&) = delete;

	// Can be moved
	Music(Music&& other) noexcept {
		music_ = other.music_;
		other.music_ = nullptr;
	}
	Music& operator=(Music&& other) noexcept {
		this->~Music();
		music_ = other.music_;
		other.music_ = nullptr;
		return *this;
	}

	virtual ~Music() {
		if (music_ != nullptr)
			Mix_FreeMusic(music_);
	}



	// To play the song an amount of times (defaults to infinite times)
	inline void play(int loops = -1) const {
		assert(loops >= -1 && music_ != nullptr);
		Mix_PlayMusic(music_, loops);
	}

	// To change the volume (ranges from 0 to 128 both included)
	inline static int setMusicVolume(int volume) {
		assert(volume >= 0 && volume <= 128);
		return Mix_VolumeMusic(volume);
	}

	// Pause/resume/stop
	inline static void haltMusic() { Mix_HaltMusic(); }
	inline static void pauseMusic() { Mix_PauseMusic(); }
	inline static void resumeMusic() { Mix_ResumeMusic(); }



};

