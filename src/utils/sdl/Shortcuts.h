#pragma once

#include <SDL.h>
#include <string>
#include "../vector2D.h"


// To build a SDL_Rect from the parameters
inline SDL_Rect sdlrect(Vector2D& pos, float w, float h) {
	return {
		static_cast<int>(pos.getX()),
		static_cast<int>(pos.getY()),
		static_cast<int>(w),
		static_cast<int>(h)
	};
}
inline SDL_Rect sdlrect(float x, float y, float w, float h) {
	return {
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(w),
		static_cast<int>(h)
	};
}


// To build a SDL_Color from a hex number or the string of a hex number
inline SDL_Color sdlcolor(unsigned long num) {
	return {
	 static_cast<Uint8>((num >> 24) & 0xff),
	 static_cast<Uint8>((num >> 16) & 0xff),
	 static_cast<Uint8>((num >> 8) & 0xff),
	 static_cast<Uint8>(num & 0xff)
	};
}
inline SDL_Color sdlcolor(std::string strnum) {
	return sdlcolor(stoul(strnum.erase(0, 2), nullptr, 16));
}


// Expands SDL_Color into its rgba components 
#define COLOREXP(color) color.r, color.g, color.b, color.a