#pragma once

#include <SDL_ttf.h>
#include <string>
#include <cassert>
#include "Shortcuts.h"

class Font {
private:
	TTF_Font* font_;

public:
	// Constructors
	Font(const std::string& fileName, int size) {
		font_ = TTF_OpenFont(fileName.c_str(), size);
		if (font_ == nullptr) {
			throw "Couldn't load font: " + fileName;
		}
	}
	// To avoid copies 
	Font(const Font& other) = delete;
	Font& operator=(const Font& other) = delete;

	// Can be moved
	Font& operator=(Font&& other) noexcept {
		this->~Font();
		font_ = other.font_;
		other.font_ = nullptr;
		return *this;
	}
	Font(Font&& other) noexcept {
		font_ = other.font_;
		other.font_ = nullptr;
	}

	virtual ~Font() {
		if (font_ != nullptr) TTF_CloseFont(font_);
	}


	// Render text with fgColor
	inline SDL_Surface* renderText(const std::string& text, SDL_Color fgColor) const {
		assert(font_ != nullptr, "Font wasn't loaded");
		return TTF_RenderUTF8_Solid(font_, text.c_str(), fgColor);
	}
	// Render text with fgColor and bgColor background
	inline SDL_Surface* renderText(const std::string& text, SDL_Color fgColor, SDL_Color bgColor) const {
		assert(font_ != nullptr, "Font wasn't loaded");
		return TTF_RenderUTF8_Shaded(font_, text.c_str(), fgColor, bgColor);
	}


}
;
