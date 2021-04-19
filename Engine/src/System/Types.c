#include "Types.h"

SDL_Rect InitRect(int x, int y, int w, int h){
    SDL_Rect rect;
    
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    return rect;
}

Vector2d InitVector2d(double x, double y){
    Vector2d vector;

    vector.x = x;
    vector.y = y;

    return vector;
}

Vector2i InitVector2i(int x, int y){
    Vector2i vector;

    vector.x = x;
    vector.y = y;

    return vector;
}