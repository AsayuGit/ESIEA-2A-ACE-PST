#include "CommunFunctions.h"

Vector2i RectTileToCorrdinate(SDL_Rect SrcRect, Vector2i SurfaceBounds, int TileX, int TileY){
    Vector2i Coordinates;
/*
    SrcRect.y = ((TileID * SrcRect.w) / SurfaceBounds.x);
    SrcRect.x = (TileID - (SrcRect.y * (SurfaceBounds.x / SrcRect.w))) * SrcRect.w;
    SrcRect.y *= SrcRect.h;

    Coordinates.y = SrcRect.y;
    Coordinates.x = SrcRect.x; */

    Coordinates.x = TileX * SrcRect.w;
    Coordinates.y = TileY * SrcRect.h;

    return Coordinates;
}