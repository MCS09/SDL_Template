#include <iostream>

#include "utils/sdl/SDLUtils.h"
#include "utils/sdl/InputHandler.h"
#include "../src/checkMemLeaks.h"
using namespace std;

#include "fsm/GameStateMachine.h"
#include "fsm/GameStates/TextInputTest.h"

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	bool openGL = false;
	SDLUtils* sdl = SDLUtils::init("Testing", 1280, 720, openGL);
	InputHandler* ih = InputHandler::init();
	

	GameStateMachine* fsm = GameStateMachine::init();
	fsm->pushState(new TextInputTest());


	
	bool running = true;
	while (running) {
		if (ih->isWindowClosed() || ih->isKeyDown(SDLK_ESCAPE))
			running = false;
		else
			fsm->run();
		
	}


	return 0;
}


