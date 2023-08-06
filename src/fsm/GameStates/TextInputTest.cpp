#include "TextInputTest.h"


TextInputTest::TextInputTest() : GameState() {
	stateID = STATEID;
	txt = nullptr;
	font = new Font("resources/Roboto.ttf", 20);
	SDL_StopTextInput();
}

TextInputTest::~TextInputTest() {
	delete font;
	if (txt != nullptr) {
		delete txt;
		txt = nullptr;
	}
}


void TextInputTest::update(float frameTime) {
	if (SDL_IsTextInputActive()) {
		if (txt != nullptr) {
			delete txt;
			txt = nullptr;
		}

		if (!ih->getInputText().empty()) txt = new Texture(sdl->renderer(), ih->getInputText(), *font, sdlcolor(0xFFFFFFFF));
	}
	else {
		if (txt != nullptr) {
			delete txt;
			txt = nullptr;
		}
	}
}

void TextInputTest::render() {
	if (txt != nullptr) {
		txt->render(sdlrect(TEXTPOS, TEXTPOS, txt->width(), txt->height()));


		SDL_SetRenderDrawColor(sdl->renderer(), sdlcolor(0xFFFFFFFF).r, sdlcolor(0xFFFFFFFF).r, sdlcolor(0xFFFFFFFF).b, sdlcolor(0xFFFFFFFF).a);
		SDL_Rect aaa = sdlrect(TEXTPOS, TEXTPOS, txt->width(), txt->height());
		SDL_RenderDrawRect(sdl->renderer(), &aaa);
	}
}

void TextInputTest::handleEvents() {
	if (ih->isKeyDown(SDLK_RETURN)) {
		if (!SDL_IsTextInputActive()) {
			SDL_StartTextInput();
			std::cout << "Started text input" << std::endl;
			ih->refresh();
		}
	}

}