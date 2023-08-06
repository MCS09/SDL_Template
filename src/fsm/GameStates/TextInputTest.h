#pragma once

#include "../GameState.h"

class TextInputTest : public GameState {
private:
	const std::string STATEID = "Text input test";
	const int TEXTPOS = 300;


	Texture* txt;
	Font* font;

public:
	TextInputTest();
	~TextInputTest();

	void update(float frameTime);
	void render();
	void handleEvents();
};

