#include <iostream>

#include "utils/sdl/SDLUtils.h"
#include "utils/sdl/InputHandler.h"
#include "../src/checkMemLeaks.h"
using namespace std;

#include "fsm/GameStateMachine.h"
#include "fsm/GameStates/TextInputTest.h"

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SDLUtils* sdl = SDLUtils::init("Testing", 1280, 720, false);
	InputHandler* ih = InputHandler::init();
	

	GameStateMachine* fsm = GameStateMachine::init();
	fsm->pushState(new TextInputTest());


	const int MAX_FRAMERATE = 15;
	const int TICKS_PER_FRAME = 1000 / MAX_FRAMERATE;
	Timer frameRateCap;
	sdl->getTimer().start();
	int frameTicks = 0;

	bool running = true;
	while (running) {
		frameRateCap.start();
		
		if (ih->isWindowClosed() || ih->isKeyDown(SDLK_ESCAPE))
			running = false;
		else
			fsm->run(frameTicks);
		

		frameTicks = frameRateCap.getTicks();
		if (frameTicks < TICKS_PER_FRAME)
			SDL_Delay(TICKS_PER_FRAME - frameTicks);

	}


	return 0;
}


