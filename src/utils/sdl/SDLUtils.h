#pragma once

#include "../Singleton.h"
#include <string>
#include <SDL.h>
#include <unordered_map>
#include "../Timer.h"
#include "Texture.h"
#include "Font.h"
#include "SoundEffect.h"
#include "Music.h"

#include <SDL_opengl.h>


class SDLUtils : public Singleton<SDLUtils> {
	friend Singleton<SDLUtils>;
	
public:
	template<typename T>
	using resource_map = std::unordered_map<std::string, T>;

	template<typename T>
	class resource_map_wrapper {
	public:
		resource_map<T>& map_;
		std::string name_;

		resource_map_wrapper(resource_map<T>& map, std::string name) : map_(map), name_(name) { }

		inline auto& at(std::string fileName) {
			try { return map_.at(fileName); }

			// File doesn't exist in map
			catch (std::out_of_range& e) {
				throw "'" + fileName + "' does not exists in '" + name_ + "'";
			}
			// File exists but there's an error accessing it
			catch (std::exception& e) {
				throw "Error accessing '" + fileName + "' of '" + name_ + "'. Error: " + e.what();
			}
		}
	};


private:
	static const int DEFAUDIOCH_ = 4;
	SDLUtils();
	SDLUtils(std::string windowTitle, int w, int h);
	SDLUtils(std::string windowTitle, int w, int h, bool opengGL);
	SDLUtils(std::string windowTitle, int w, int h, int audioChannels);
	SDLUtils(std::string windowTitle, int w, int h, bool openGL, int audioChannels);
	//SDLUtils(std::string windowTitle, int w, int h, std::string fileName, int audioChannels = 4);

	std::string error_;
	Timer timer_;

	// openGL
	bool openGL;
	SDL_GLContext glContext;
	//GLuint gProgramID;
	//GLint gVertexPos2DLocation;
	//GLuint gVBO;
	//GLuint gIBO;


	// Window
	std::string windowTitle_;
	int windW_;
	int windH_;
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	void initWindow();

	// Resources
	std::unordered_map<std::string, Texture> images_;
	std::unordered_map<std::string, Font> fonts_;
	int audioChannels_;
	std::unordered_map<std::string, SoundEffect> sounds_;
	std::unordered_map<std::string, Music> musics_;
	void initResources();
	//void loadResources(std::string fileName);

	resource_map_wrapper<Texture> imagesWrapper_;
	resource_map_wrapper<Font> fontsWrapper_;
	resource_map_wrapper<SoundEffect> soundsWrapper_;
	resource_map_wrapper<Music> musicsWrapper_;



public:
	// To avoid copying/moving the instance
	SDLUtils(SDLUtils&) = delete;
	SDLUtils(SDLUtils&&) = delete;
	SDLUtils& operator=(SDLUtils&) = delete;
	SDLUtils& operator=(SDLUtils&&) = delete;
	~SDLUtils();


	inline Timer& getTimer() { return timer_; }


	inline SDL_Renderer* renderer() { return renderer_; }
	// Clears the renderer with a given SDL_Color (default to black)
	inline void clearRenderer(SDL_Color bg = sdlcolor(0x000000FF)) {
		if (!openGL) {
			SDL_SetRenderDrawColor(renderer_, COLOREXP(bg));
			SDL_RenderClear(renderer_);
		}
		else {
			/*glViewport(0, 0, windW_, windH_);
			glClearColor(1.f, 0.f, 1.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT);*/

			SDL_GL_SwapWindow(window_);
		}
	}
	// Presents the renderer
	inline void presentRenderer() { 
		if(!openGL) 
			SDL_RenderPresent(renderer_); 
		else {

		}
	}

	inline SDL_Window* window() { return window_; }
	inline int width() { return windW_; }
	inline int height() { return windH_; }
	inline void toggleFullScreen() {
		auto flags = SDL_GetWindowFlags(window_);
		if (flags & SDL_WINDOW_FULLSCREEN)  SDL_SetWindowFullscreen(window_, 0);
		else SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
	}

	inline auto& images() { return imagesWrapper_; }
	inline auto& fonts() { return fontsWrapper_; }
	inline auto& sounds() { return soundsWrapper_; }
	inline auto& musics() { return musicsWrapper_; }


	// Adds a new resource of the specified type (T) and the specified args to the map of the specified wrapper
	template<typename T, typename... Ts>
	inline void load(resource_map_wrapper<T> wrapper, std::string key, Ts&&... args) {
		wrapper.map_.emplace(key, T(std::forward<Ts>(args)...));
	}
};


// To shorten SDLUtils::instance()->method() to sdlutils().method()
inline SDLUtils& sdlutils() { return *SDLUtils::instance(); }