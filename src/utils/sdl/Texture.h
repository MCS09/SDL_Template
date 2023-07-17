#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <cassert>
#include <iostream>
#include "Font.h"

class Texture {
private:
	SDL_Texture* texture_;
	SDL_Renderer* renderer_;
	int width_;
	int height_;
	int fw_;
	int fh_;
	float opacity_;

	// Generates texture with a text written in the specified font
	void textureFromText(SDL_Renderer* renderer, const std::string& text, const Font& font, const SDL_Color* fgColor, const SDL_Color* bgColor = nullptr);


public:
	// Construct from image
	Texture(SDL_Renderer* renderer, const std::string& fileName);

	// Construct from text
	Texture(SDL_Renderer* renderer, const std::string& text, const Font& font, const SDL_Color& fgColor);
	// Construct from text with background
	Texture(SDL_Renderer* renderer, const std::string& text, const Font& font, const SDL_Color& fgColor, const SDL_Color& bgColor);

	// To avoid copies 
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// Can be moved
	Texture& operator=(Texture&& other) noexcept;
	Texture(Texture&& other) noexcept;
	
	virtual ~Texture() {
		if (texture_ != nullptr) SDL_DestroyTexture(texture_);
	}

	inline int width() const { return width_; }
	inline int height() const { return height_; }
	inline int fwidth() const { return fw_; }
	inline int fheight() const { return fh_; }

	
	// Renders

	// Renders part of the texture (src) to a destination rectangle (dest)
	// with a rotation (angle) around the point p (of dest), and flips according
	// to the value of flip. If 'p'is nullptr, it rotates around the center
	inline void render(const SDL_Rect& src, const SDL_Rect& dest, double angle, const SDL_Point* p = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
		assert(texture_ != nullptr);
		SDL_RenderCopyEx(renderer_, texture_, &src, &dest, angle, p, flip);
	}

	// Renders part of the texture (src) to a destination rectangle (dest)
	inline void render(const SDL_Rect& src, const SDL_Rect& dest) {
		assert(texture_ != nullptr);
		SDL_RenderCopy(renderer_, texture_, &src, &dest);
	}

	// Renders the complete texture at the position (x,y).
	inline void render(int x, int y) {
		SDL_Rect dest = { x, y, width_, height_ };
		render(dest);
	}
	inline void render(Vector2D v) {
		SDL_Rect dest = { v.getX(), v.getY(), width_, height_};
		render(dest);
	}

	// Renders the complete texture at a destination rectangle (dest)
	inline void render(const SDL_Rect& dest) {
		SDL_Rect src = { 0, 0, width_, height_ };
		render(src, dest);
	}

	// Renders the complete texture at a destination rectangle (dest), with rotation
	inline void render(const SDL_Rect& dest, float rotation) {
		SDL_Rect src = { 0, 0, width_, height_ };
		render(src, dest, rotation);
	}

	inline void setOpacity(float perc) {
		assert(texture_ != nullptr);
		SDL_SetTextureAlphaMod(texture_, (int)((255 * perc) / 100));
		opacity_ = perc;
	}

	inline float getOpacity() {
		assert(texture_ != nullptr);
		return opacity_;
	}

	inline void renderFrame(const SDL_Rect& dest, int col, int row, float angle = 0) {
		SDL_Rect src;
		src.x = fw_ * col;
		src.y = fh_ * row;
		src.w = fw_;
		src.h = fh_;
		render(src, dest, angle);
	}

	SDL_Texture* getTexture() const { return texture_; }
};
