#include "include.h"
#include "Scenes.h"

int main(int argc, char* argv){

    DisplayDevice* DisplayDevice;
    InputDevice* InputDevice;

    CourtroomContext* CourtroomContext;
    CourtroomContext = NULL;

    InitSDL();
    DisplayDevice = CreateDisplayDevice(256, 192, "Ace YAY");
    InputDevice = InitInputs(false);
    // We start the courtroom Scene
    Scene_Courtroom(DisplayDevice, InputDevice, CourtroomContext);

    free(CourtroomContext);

    free(DisplayDevice);

    return 0;
}