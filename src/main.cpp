#include <iostream>

#include "utils/sdl/SDLUtils.h"
#include "utils/sdl/InputHandler.h"
#include "../src/checkMemLeaks.h"
using namespace std;

#include "../src/BaseLevel.h"

#include <tmxlite/Map.hpp>


int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SDLUtils* sdl = SDLUtils::init("Testing", 1280, 720);
	InputHandler* ih = InputHandler::init();
	
	BaseLevel* level = new BaseLevel("Pallet", "resources/untitled.tmx");


	bool running = true;
	while (!ih->isWindowClosed() && !ih->isKeyDown(SDLK_ESCAPE)) {
		sdl->clearRenderer();

		ih->refresh();

		if (ih->isAnyKeyDown()) 
			cout << ih->getKeyPressed() << " ";

		level->render();
		sdl->presentRenderer();
	}

	delete level;
	return 0;
}
