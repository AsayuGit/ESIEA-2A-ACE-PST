#include "include.h"
#include "Scenes.h"

int main(int argc, char* argv){

    DisplayDevice* DisplayDevice;
    InputDevice* InputDevice;

    CourtroomContext* CourtContext;
    CourtContext = (CourtroomContext*)malloc(sizeof(CourtroomContext));

    InitSDL();
    DisplayDevice = CreateDisplayDevice(256, 192, "Ace YAY");
    InputDevice = InitInputs(false);
    // We start the courtroom Scene
    Scene_Courtroom(DisplayDevice, InputDevice, CourtContext);

    free(CourtContext);

    free(DisplayDevice);

    return 0;
}