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

	virtual bool onEnter() {
		std::cout << "Entering " + stateID << std::endl;
		return true;
	};
	virtual bool onExit() { 
		std::cout << "Exiting " + stateID << std::endl; 
		return true;
	};
	virtual std::string getStateID() { return stateID; };

	void run(float frameTime);

	virtual void update(float frameTime) {};
	virtual void render() {};
	virtual void handleEvents() {};

	virtual void reset() {};

	Manager* getManager() { return manager; };
};

