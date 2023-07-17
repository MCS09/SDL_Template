#include "SDLUtils.h"
#include <cassert>

SDLUtils::SDLUtils() : SDLUtils("Default constructor", 600, 400) { }

SDLUtils::SDLUtils(std::string windowTitle, int width, int height, int audioChannels) :
	windowTitle_(windowTitle), windW_(width), windH_(height), audioChannels_(audioChannels),
	imagesWrapper_(images_, "Textures"), fontsWrapper_(fonts_, "Fonts"), soundsWrapper_(sounds_, "Sounds"), musicsWrapper_(musics_, "Musics")
{
	initWindow();
	initResources();
}

//SDLUtils::SDLUtils(std::string windowTitle, int width, int height, int audioChannels, std::string filename) : SDLUtils(windowTitle, width, height, audioChannels)
//{
//	loadResources(filename);
//}

SDLUtils::~SDLUtils() {
	closeResources();
	closeWindow();
}

void SDLUtils::initWindow() {
	// Initialize SDL
	int sdlInit = SDL_Init(SDL_INIT_EVERYTHING);
	assert(sdlInit == 0);

	// Window
	window_ = SDL_CreateWindow(windowTitle_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windW_, windH_, SDL_WINDOW_SHOWN);
	assert(window_ != nullptr);

	// Renderer
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(renderer_ != nullptr);
}

void SDLUtils::closeWindow() {
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void SDLUtils::initResources() {
	// initialize SDL_ttf
	int ttfInit = TTF_Init();
	assert(ttfInit == 0);

	// initialize SDL_image
	int imgfInit = IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	assert(imgfInit != 0);

	// initialize SDL_Mixer
	int mixOpenAudioInit = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	assert(mixOpenAudioInit == 0);
	int mixInit = Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	assert(mixInit != 0);

	SoundEffect::setNumberofChannels(audioChannels_);
}

void SDLUtils::closeResources() {
	musics_.clear();
	sounds_.clear();
	images_.clear();
	fonts_.clear();

	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
}
