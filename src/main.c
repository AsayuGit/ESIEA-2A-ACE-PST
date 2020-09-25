#include "inlcude.h"

int main(int argc, char* argv){

    DisplayDevice* DisplayDevice;
    Vector2i Resolution = {640, 480};

    InitSDL();
    DisplayDevice = CreateDisplayDevice(&Resolution, "Ace YAY");

    while (1){
        SDL_RenderClear(DisplayDevice->Renderer);
        // Stuff
        SDL_RenderPresent(DisplayDevice->Renderer);
    }

    free(DisplayDevice);

    return 0;
}