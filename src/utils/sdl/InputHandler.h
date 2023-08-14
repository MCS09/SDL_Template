#pragma once

#include "../Singleton.h"
#include <SDL.h>
#include <array>
#include <vector>


class InputHandler : public Singleton<InputHandler> {
	friend Singleton<InputHandler>;

private:
	// Types of events
	enum EVENTS {
		e_closeWindow,

		e_keyUp,
		e_keyDown,

		e_mouseMoving,
		e_mousebuttonDown,

		e_joyAxisMotion,
		e_joyButtonUp,
		e_joyButtonDown,
		//e_joyDevice,

		e_lastEventType
	};
	bool eventTriggered_[e_lastEventType];

	// Mouse and keyboard states
	std::pair<Sint32, Sint32> mousePos_;
	bool mbState_[3];
	const Uint8* kbState_;

	// Joysticks
	const int JOYSTICK_DEAD_ZONE = 8000;
	bool joystickConnected_;
	std::vector<SDL_Joystick*> joysticks_;								// All conected joysticks
	std::vector<std::pair<Vector2D, Vector2D>> joysticksValues_;		// Axes values
	std::vector<std::vector<bool>> jbState_;							// Button states


	// Text input
	std::string text_;
	const char* composition_;
	Sint32 cursor_;
	Sint32 selection_len_;


	InputHandler() {
		kbState_ = SDL_GetKeyboardState(0);

		text_ = "";
		resetText();

		initJoystick();

		clear();
	}

	
	// Checks if there are any joysticks connected and initializes all of them
	inline void initJoystick() {
		int numJoysticks = SDL_NumJoysticks();

		// No joysticks connected
		if (numJoysticks < 1) {
			std::cout << "No joysticks connected" << std::endl;
			joystickConnected_ = false;
		}
		// At least 1 joystick connected
		else {
			for (int i = 0; i < numJoysticks; i++) {
				SDL_Joystick* joy = SDL_JoystickOpen(i);
				if (joy != NULL) {
					// Adds the joystick to the vector of joysticks
					joysticks_.push_back(SDL_JoystickOpen(i));

					// Adds a set of axes values to the joystick
					joysticksValues_.push_back({ {0,0} , {0,0} });

					// Adds a set of buttons to the joystick
					std::vector<bool> tempButtons(SDL_JoystickNumButtons(joy), false);
					jbState_.push_back(tempButtons);

				}
				else std::cout << "Unable to open game joystick, " << SDL_GetError() << std::endl;				
			}

			// Start lisening to joystick events
			SDL_JoystickEventState(SDL_ENABLE);
			joystickConnected_ = true;
		}
	}

	// Clears all of the input
	inline void clear() {
		for (auto& e : eventTriggered_) e = false;
		for (auto& m : mbState_) m = false;
	}


	// Triggers the keyboard press/release event
	inline void onKeyDown() { eventTriggered_[e_keyDown] = true; }
	inline void onKeyUp() { eventTriggered_[e_keyUp] = true; }

	// Triggers the mouse motion event and sets the mouse position
	inline void onMouseMotion(const SDL_Event& event) {
		eventTriggered_[e_mouseMoving] = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;
	}
	// Triggers the mouse button event and changes the mouse button states
	inline void onMouseButtonChange(const SDL_Event& event, bool isDown) {
		eventTriggered_[e_mousebuttonDown] = true;

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

	// Triggers the window close event
	inline void handleWindowEvent(const SDL_Event& event) {
		switch (event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			eventTriggered_[e_closeWindow] = true;
			break;
		default:
			break;
		}
	}

	// Changes the values from the sticks
	inline void joyAxisMotionEvent(const SDL_Event& event) {
		int joystick = event.jaxis.which;
		int value = event.jaxis.value;

		// Left stick X 
		if (event.jaxis.axis == 0) {
			if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].first.setX(1);
			else if (value < -JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].first.setX(-1);
			else joysticksValues_[joystick].first.setX(0);
		}
		// Left stick Y 
		if (event.jaxis.axis == 1) {
			if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].first.setY(1);
			else if (value < -JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].first.setY(-1);
			else joysticksValues_[joystick].first.setY(0);
		}
		// Right stick X 
		if (event.jaxis.axis == 2) {
			if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].second.setX(1);
			else if (value < -JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].second.setX(-1);
			else joysticksValues_[joystick].second.setX(0);
		}
		// Right stick Y 
		if (event.jaxis.axis == 3) {
			if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].second.setY(1);
			else if (value < -JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].second.setY(-1);
			else joysticksValues_[joystick].second.setY(0);
		}

	}

	// Triggers the joystick button press event and changes the joystick button states
	inline void joyButtonDown(const SDL_Event& event) {
		eventTriggered_[e_joyButtonDown] = true; 

		int joystick = event.jaxis.which;
		jbState_[joystick][event.jbutton.button] = true;

	}
	// Triggers the joystick button release event and changes the joystick button states
	inline void joyButtonUp(const SDL_Event& event) {

		eventTriggered_[e_joyButtonUp] = true;

		int joystick = event.jaxis.which;
		jbState_[joystick][event.jbutton.button] = false;
	}


public:
	enum MOUSEBUTTON : uint8_t { LEFT = 0, MIDDLE = 1, RIGHT = 2 };
	enum STICKS { leftX, leftY, rightX, rightY };

	virtual ~InputHandler() {
		for (auto cont : joysticks_) {
			SDL_JoystickClose(cont);
			cont = NULL;
		}
	}

	inline void update(const SDL_Event& event) {
		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown();
			break;
		case SDL_KEYUP:
			onKeyUp();
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
		case SDL_JOYAXISMOTION:
			joyAxisMotionEvent(event);
			break;
		case SDL_JOYBUTTONDOWN:
			joyButtonDown(event);
			break;
		case SDL_JOYBUTTONUP:
			joyButtonUp(event);
			break;

		default:
			break;
		}

		// If text input is active
		if (SDL_IsTextInputActive()) {
			// Removes the last character
			if(isKeyDown(SDLK_BACKSPACE))
				removeLastChar();

			// Disables text input and clears the text
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

	// Returns true if the close window event has been triggered
	inline bool isWindowClosed() { return eventTriggered_[e_closeWindow]; }


	// Returns true if the key press event has been triggered
	inline bool keyDownEvent() { return eventTriggered_[e_keyDown]; }

	// Returns true if the specified key has been pressed
	inline bool isKeyDown(SDL_Scancode key) { return keyDownEvent() && kbState_[key] == 1; }
	inline bool isKeyDown(SDL_Keycode key) { return isKeyDown(SDL_GetScancodeFromKey(key)); }


	// Returns true if the key release event has been triggered
	inline bool keyUpEvent() { return eventTriggered_[e_keyUp]; }

	// Returns true if the specified key has been released
	inline bool isKeyUp(SDL_Scancode key) { return keyUpEvent() && kbState_[key] == 0; }
	inline bool isKeyUp(SDL_Keycode key) { return isKeyUp(SDL_GetScancodeFromKey(key)); }
	

	// Reeturns true if the mouse moving event has been triggered
	inline bool isMouseMoving() { return eventTriggered_[e_mouseMoving]; }
	// Returns true if the mouse button press has been triggered
	inline bool isMouseButtonDown() { return eventTriggered_[e_mousebuttonDown]; }
	// Returns the mouse position
	inline const std::pair<Sint32, Sint32>& getMousePos() { return mousePos_; }
	// Returns true if the specified mouse button has been pressed
	inline int isMouseButtonDown(MOUSEBUTTON b) { return mbState_[b]; }


	// Returns the inputted text
	inline std::string getInputText() { return text_; }
	// Clears the inputted text
	inline void resetText() { text_.clear(); }
	// Removes the last char from the inputted text
	inline void removeLastChar() { 
		if(!text_.empty()) 
			text_.pop_back(); 
	}


	// Returns true if there's at least 1 joystick conneceted
	inline bool isjoystickConnected() { return joystickConnected_; }

	// Returns X value from the specified stick of the specified joystick  
	inline int joystickXValue(int joystick, int stick) {
		if (!joysticksValues_.empty()) {
			if (stick == 1) return joysticksValues_[joystick].first.getX();
			else if (stick == 2) return joysticksValues_[joystick].second.getX();
			else return 0;
		}
	}
	// Returns Y value from the specified stick of the specified joystick  
	inline int joystickYValue(int joystick, int stick) {
		if (!joysticksValues_.empty()) {
			if (stick == 1) return joysticksValues_[joystick].first.getY();
			else if (stick == 2) return joysticksValues_[joystick].second.getY();
			else return 0;
		}
	}


	// Returns true if the joystick button press event has been triggered
	inline bool joystickButtonDownEvent() { return eventTriggered_[e_joyButtonDown]; }
	// Returns true if the joystick button release event has been triggered
	inline bool joystickButtonUpEvent() { return eventTriggered_[e_joyButtonUp]; }

	// Returns true if the specified joystick specified button has been pressed
	bool isJoystickButtonDown(int joystick, int button) { return jbState_[joystick][button]; }



};


// To shorten InputHandler::instance()->method() to inputHandler().method()
inline InputHandler& inputHandler() { return *InputHandler::instance(); }
