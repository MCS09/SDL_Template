#pragma once

#include <list>
#include <SDL.h>
#include <string>

#include "../ecs/Manager.h"
#include "../utils/sdl/SDLUtils.h"
#include "../utils/sdl/InputHandler.h"

class GameState {
protected:
	Manager* manager;
	std::string stateID;
	SDLUtils* sdl;
	InputHandler* ih;


public:
	GameState(std::string stateID = "") : 
		manager(new Manager()), stateID(stateID), sdl(SDLUtils::instance()), ih(InputHandler::instance()) {};
	virtual ~GameState() { delete manager; };

	virtual bool onEnter() { return false; };
	virtual bool onExit() { return false; };
	virtual std::string getStateID() { return stateID; };

	virtual void run(float frameTime);

	virtual void update(float frameTime) {};
	virtual void render() {};
	virtual void handleEvents() {};

	virtual void reset() {};

	Manager* getManager() { return manager; };
};

