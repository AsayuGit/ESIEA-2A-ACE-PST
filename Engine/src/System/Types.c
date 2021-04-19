#include "Types.h"

SDL_Rect InitRect(int x, int y, int w, int h){
    SDL_Rect rect = {x, y, w, h};
    return rect;
}

Vector2d InitVector2d(double x, double y){
    Vector2d vector = {x, y};
    return vector;
}

Vector2i InitVector2i(int x, int y){
    Vector2i vector = {x, y};
    return vector;
}