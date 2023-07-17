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
	
	sdl->load(sdl->images(), "Hack", Texture(sdl->renderer(), "resources/hack1.gif"));
	/*BaseLevel* level = new BaseLevel("Pallet", "resources/tiles/pallet.tmx");*/

	tmx::Map map_;
	map_.load("resources/tiles/pallet.tmx");


	bool running = true;
	while (!ih->isWindowClosed() && !ih->isKeyDown(SDLK_ESCAPE)) {
		sdl->clearRenderer();

		ih->refresh();

		if (ih->isAnyKeyDown()) 
			cout << ih->getKeyPressed() << " ";

		
		sdl->images().at("Hack").render(0,0);
		sdl->presentRenderer();
	}

	return 0;
}
