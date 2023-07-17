#include "GameStateMachine.h"

#include "../utils/sdl/InputHandler.h"
#include "../checkMemLeaks.h"


// Adds a new state on top of the stack
void GameStateMachine::pushState(GameState* state) {
	gameStates.push(state);
}

// Changes the state on top of the stack and adds the new one on top
void GameStateMachine::changeState(GameState* state) {
	// If the stack isn't empty
	if (!gameStates.empty()) {
		// If the current state isn't the same "type" 
		// as the new one, it gets deleted from the stack
		if (gameStates.top()->getStateID() != state->getStateID()) popState();
	}	

	// Then, the new state is added to the top of the stack
	pushState(state);
}

// Deletes the state on top of the stack
void GameStateMachine::popState() {
	// If the stack isn't empty
	if (!gameStates.empty()) {
		// If the sate could be exited, it gets removed 
		// from the stack and deleted (in the next loop)
		if (gameStates.top()->onExit()) {
			deleting = gameStates.top();
			gameStates.pop();			
		}
	}
}

// Runs the current state's loop
void GameStateMachine::run(float frameTime) {
	if (!gameStates.empty()) gameStates.top()->run(frameTime);

	// Deletes the previously exited state
	if(deleting != nullptr) {
		delete deleting;
		deleting = nullptr;
	}
};