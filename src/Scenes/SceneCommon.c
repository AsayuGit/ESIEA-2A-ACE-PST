#include "SceneCommon.h"

void MoveTile(SDL_Rect* Tile, Vector2i* Coordinates){
    Tile->x = Tile->w * Coordinates->x;
    Tile->y = Tile->h * Coordinates->y;
}