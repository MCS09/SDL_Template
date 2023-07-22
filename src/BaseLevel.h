#pragma once

#include "utils/sdl/SDLUtils.h"
#include <map>
#include <tmxlite/Map.hpp>


struct tile {
	Texture* tileset;
	int posX;
	int posY;
	int tsX;
	int tsY;
	int width;
	int height;

	tile() {};
	tile(Texture* tset, int x = 0, int y = 0, int tsX = 0, int tsY = 0, int w = 0, int h = 0);
	void draw();
};

class BaseLevel {
private:
	SDLUtils* sdl;

    std::string name_;
	tmx::Map map_;
    int rows_, cols_, tileW_, tileH_;
    std::vector<tile> tiles;
    std::map<int, Texture*> tilesets_;


public:
    BaseLevel(const std::string& name, const std::string& path);
	~BaseLevel() { };
    void render();

private:
	void loadDims();
	void loadTilesets();
	void loadTiles();

};

