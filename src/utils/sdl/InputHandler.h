#pragma once

#include "../Singleton.h"
#include <SDL.h>
#include <array>

// Missing Joystick support (Chapter 4 of 'SDL Game Development')

class InputHandler : public Singleton<InputHandler> {
	friend Singleton<InputHandler>;

private:
	bool closeWindow_;
	bool isKeyUp_;
	bool isKeyDown_;
	bool isMouseMoving_;
	bool isMouseButtonDown_;
	std::pair<Sint32, Sint32> mousePos_;
	std::array<bool, 3> mbState_;
	const Uint8* kbState_;

	std::string text_;
	const char* composition_;
	Sint32 cursor_;
	Sint32 selection_len_;

	InputHandler() {
		text_ = "";
		resetText();
		kbState_ = SDL_GetKeyboardState(0);
		clear();
	}


	inline void clear() {
		closeWindow_ = false;
		isKeyDown_ = false;
		isKeyUp_ = false;
		isMouseButtonDown_ = false;
		isMouseMoving_ = false;
		for (bool m : mbState_) m = false;
	}

	inline void onKeyDown(const SDL_Event& event) { isKeyDown_ = true; }

	inline void onKeyUp(const SDL_Event& event) {  isKeyUp_ = true; }

	inline void onMouseMotion(const SDL_Event& event) {
		isMouseMoving_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;
	}

	inline void onMouseButtonChange(const SDL_Event& event, bool isDown) {
		isMouseButtonDown_ = true;

		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = isDown;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = isDown;
			break;
		default:
			break;
		}
	}

	inline void handleWindowEvent(const SDL_Event& event) {
		switch (event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			closeWindow_ = true;
			break;
		default:
			break;
		}
	}

	

public:
	enum MOUSEBUTTON : uint8_t { LEFT = 0, MIDDLE = 1, RIGHT = 2 };

	virtual ~InputHandler() { }

	inline void update(const SDL_Event& event) {
		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;
		case SDL_WINDOWEVENT:
			handleWindowEvent(event);
			break;
		case SDL_TEXTINPUT:
			text_ += event.text.text;
			break;
		case SDL_TEXTEDITING:
			composition_ = event.edit.text;
			cursor_ = event.edit.start;
			selection_len_ = event.edit.length;
			break;
		default:
			break;
		}

		if (SDL_IsTextInputActive()) {
			if(isKeyDown(SDLK_BACKSPACE))
				removeLastChar();

			else if (isKeyDown(SDLK_RETURN)) {
				SDL_StopTextInput();
				resetText();
				refresh();
				std::cout << "Ended text input" << std::endl;
			}


		}
	}
	inline void refresh() {
		SDL_Event event;
		clear();
		while (SDL_PollEvent(&event)) 
			update(event);
	}



	inline bool isWindowClosed() { return closeWindow_; }


	inline bool keyDownEvent() { return isKeyDown_; }
	inline bool isKeyDown(SDL_Scancode key) { return keyDownEvent() && kbState_[key] == 1; }
	inline bool isKeyDown(SDL_Keycode key) { return isKeyDown(SDL_GetScancodeFromKey(key)); }

	inline bool keyUpEvent() { return isKeyUp_; }
	inline bool isKeyUp(SDL_Scancode key) { return keyUpEvent() && kbState_[key] == 0; }
	inline bool isKeyUp(SDL_Keycode key) { return isKeyUp(SDL_GetScancodeFromKey(key)); }
	

	inline bool isMouseMoving() { return isMouseMoving_; }
	inline bool isMouseButtonDown() { return isMouseButtonDown_; }
	inline const std::pair<Sint32, Sint32>& getMousePos() { return mousePos_; }
	inline int getMouseState(MOUSEBUTTON b) { return mbState_[b]; }

	inline std::string getInputText() { return text_; }
	inline void resetText() { text_.clear(); }
	inline void removeLastChar() { 
		if(!text_.empty()) 
			text_.pop_back(); 
	}
};


// To shorten InputHandler::instance()->method() to inputHandler().method()
inline InputHandler& inputHandler() { return *InputHandler::instance(); }
