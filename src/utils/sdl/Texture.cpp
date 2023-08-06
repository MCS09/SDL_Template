#include "Texture.h"

// Move operations
Texture& Texture::operator=(Texture&& other) noexcept {
	this->~Texture();
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;

	return *this;
}
Texture::Texture(Texture&& other) noexcept {
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;
	fw_ = other.fw_;
	fh_ = other.fh_;
	opacity_ = other.opacity_;
}

// From image
Texture::Texture(SDL_Renderer* renderer, const std::string& fileName) {
	assert(renderer != nullptr);

	SDL_Surface* surface = IMG_Load(fileName.c_str());
	if (surface == nullptr) throw "Couldn't load " + fileName;

	texture_ = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture_ == nullptr) {
		SDL_FreeSurface(surface);
		throw "Couldn't convert surface to texture for " + fileName;
	}
	else {
		width_ = surface->w;
		height_ = surface->h;
		renderer_ = renderer;
	}
	SDL_FreeSurface(surface);
}

// From text
Texture::Texture(SDL_Renderer* renderer, const std::string& text, const Font& font, const SDL_Color& fgColor) {
	textureFromText(renderer, text, font, &fgColor);
}
// From text with background
Texture::Texture(SDL_Renderer* renderer, const std::string& text, const Font& font, const SDL_Color& fgColor, const SDL_Color& bgColor) {
	textureFromText(renderer, text, font, &fgColor, &bgColor);
}


// Aux method to create the texture from text
void Texture::textureFromText(SDL_Renderer* renderer, const std::string& text, const Font& font, const SDL_Color* fgColor, const SDL_Color* bgColor) {
	assert(renderer != nullptr);

	SDL_Surface* surface = nullptr;
	if (bgColor == nullptr) surface = font.renderText(text, *fgColor);
	else surface = font.renderText(text, *fgColor, *bgColor);

	if (surface == nullptr) throw "Couldn't create surface for text '" + text + "'";

	texture_ = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture_ == nullptr) {
		SDL_FreeSurface(surface);
		throw "Couldn't create texture for text '" + text + "'";
	}
	else {
		width_ = surface->w;
		height_ = surface->h;
		renderer_ = renderer;
	}
	

	SDL_FreeSurface(surface);
}
