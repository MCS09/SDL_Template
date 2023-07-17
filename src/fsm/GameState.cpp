#include "GameState.h"
#include "../checkMemLeaks.h"

// Main loop (executed by the FSM's, which loops the current state's loop)
void GameState::run(float frameTime) {
	ih->refresh();
	sdl->clearRenderer();


	update(frameTime);
	render();
	handleEvents();

	manager->update(frameTime);
	manager->refresh();
	manager->flushMessages();


	sdl->presentRenderer();

}
