#include "BaseLevel.h"

#include <tmxlite/TileLayer.hpp>
//#include <tmxlite/ObjectGroup.hpp>

#include "../src/checkMemLeaks.h"
using namespace tmx;



tile::tile(Texture* tset, int posX, int posY, int tsX, int tsY, int width, int height) :
	tileset(tset), posX(posX), posY(posY), tsX(tsX), tsY(tsX), width(width), height(height) { }

void tile::draw() {
	if (sdlutils().renderer() || tileset) {
        SDL_Rect src;
        src.x = tsX;
        src.y = tsY;
        src.w = width;
        src.h = height;

        SDL_Rect dest;
        dest.x = posX;
        dest.y = posY;
        dest.w = src.w;
        dest.h = src.h;

        tileset->render(src, dest);
	}
}


BaseLevel::BaseLevel(const std::string& name, const std::string& path) :
    sdl(sdlutils().instance()), name_(name), rows_(0), cols_(0), tileW_(0), tileH_(0)
{ 
    // Load and parse the Tiled map with tmxlite
    map_ = new Map();
    if (map_->load(path)) {
        loadDims();
        loadTilesets();
        loadTiles();
    }
    
}

BaseLevel::~BaseLevel() {
    //if (map_ != nullptr) delete map_;
}



void BaseLevel::render() {
    for (auto& t : tiles) t.draw();
}



void BaseLevel::loadDims() {
    // To get the map dimensions in tiles
    auto map_dimensions = map_->getTileCount();
    rows_ = map_dimensions.y;
    cols_ = map_dimensions.x;

    // To get the tiles dimensions in pixels
    auto tilesize = map_->getTileSize();
    tileW_ = tilesize.x;
    tileH_ = tilesize.y;
}

void BaseLevel::loadTilesets() {
    // Load all of the tilesets and store them in a map
    auto& map_tilesets = map_->getTilesets();

    for (auto& tileset : map_tilesets) {
        std::string tsName = tileset.getName();

        sdl->load(sdl->images(), tsName, Texture(sdl->renderer(), tileset.getImagePath()));
        tilesets_.insert({ tileset.getFirstGID(), &(sdl->images().at(tileset.getImagePath())) });
    }

}


void BaseLevel::loadTiles() {
    // Iterates through all the tilemap's layers
    auto& map_layers = map_->getLayers();
    for (auto& layer : map_layers) {
        if (layer->getType() == Layer::Type::Tile) {
            // To get all they layer's tiles
            TileLayer* tile_layer = dynamic_cast<TileLayer*>(layer.get());
            auto& layer_tiles = tile_layer->getTiles();

            // Iterates through every tile of the map
            for (int rw = 0; rw < rows_; ++rw) {
                for (int cl = 0; cl < cols_; ++cl) {
                    // Gets the tile's ID in the map and then
                    // gets the ID of that tile in the tileset
                    int tileInTilemap = cl + (rw * cols_);
                    int tileID = layer_tiles[tileInTilemap].ID;

                    // If the tileID is 0, that means it's an empty tile so it doesn't get stored.
                    // Otherwise, it looks for the tileset that tile belongs to
                    if (tileID != 0) {
                        // To check if the tile has been found in the tileset
                        bool found = false;

                        // If the first tileID in that tileset is <= than the tileID of the tile, the
                        // tile belongs to that tileset and it stops searching for it. Otherwise, the
                        // tile doesn't belong to that tileset and keeps looking for it.
                        int tilesetID = -1;
                        for (auto ts = tilesets_.begin(); ts != tilesets_.end() && !found; ++ts) {
                            if (ts->first <= tileID) {
                                found = true;
                                tilesetID = ts->first;
                            } 
                        }
                        // If the tilesetID is -1, that means no valid tileset has been found
                        // Otherwise, the tile gets stored in the tiles vector
                        if (tilesetID != -1) {
                            // Normalize the GID.
                            tileID -= tilesetID;

                            // To calculate the dimensions of the tile sheet image
                            int tilesetW = 0, tilesetH = 0;
                            SDL_QueryTexture(tilesets_[tilesetID]->getTexture(), NULL, NULL, &tilesetW, &tilesetH);
                            int tilesetCols = tilesetW / tileW_;
                            int tilesetRows = tilesetH / tileH_;

                            // Calculates the zone of the tileset where the tile is
                            int tilesetRegionX = (tileID % tilesetCols) * tileW_;
                            int tilesetRegionY = (tileID / tilesetCols) * tileH_;

                            // Calculates the world position of the tile
                            int tileX = cl * tileW_;
                            int tileY = rw * tileH_;

                            tile t(tilesets_[tilesetID], tileX, tileY, tilesetRegionX, tilesetRegionY, tileW_, tileH_);
                            tiles.push_back(t);
                        }

                    }

                    
                }
            }
        }

        
    }

}