#pragma once

#include "ecs_def.h"
#include "../checkMemLeaks.h"
class Manager;

class System {
protected:
	Manager* manager;
	bool active;
	bool isMultiplayer;
	bool isHost;

public:
	virtual ~System() {};

	void setContext(Manager* mngr) { manager = mngr; }

	virtual void receive(const ecs::Message& m) { };
	virtual void initSystem() { };
	virtual void update(float frameTime) { };
};