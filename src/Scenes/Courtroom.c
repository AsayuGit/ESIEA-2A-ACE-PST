#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"
#include "Buttons.h"

void CourtroomScenarioB(SceneContext* SContext, DialogueContext* DiagContext, int* Plot, int* CurrentCharacter, int* ReturnToDefault){
    switch (*Plot)
    {
    case -1:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "The court is now in session\nfor the trial of Ms. DEMO.", 2);
        (*Plot)++;
        break;
    case 0:
        *CurrentCharacter = Miles_Edgeworth;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution is\nready, Your Honor.", 2);
        (*Plot)++;
        break;
    case 1:
        *CurrentCharacter = Phoenix_Wright;
        BackgroundPlayAnimation(SContext, 2, NULL);
        CharacterPlayAnimation(Phoenix_Wright, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "The defence\nis ready, Your Honor.", 2);
        (*Plot)++;
        break;
    case 2:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "Mr. Edgeworth.\nPlease give the court\nyour opening statement.", 2);
        (*Plot)++;
        break;
    case 3:
        *CurrentCharacter = Miles_Edgeworth;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "Thank you, Your Honor.\nThe defandant, Ms. DEMO,\nwas at the scene of the crime.", 2);
        (*Plot)++;
        break;
    case 4:
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution has evidence\nshe committed this murder...", 1);
        (*Plot)++;
        break;
    case 5:
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "and we have a witness\nwho saw her do it.", 1);
        (*Plot)++;
        break;
    case 6:
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution sees no\nreason to doubt the facts\nof this case, Your Honor.", 1);
        (*Plot)++;
        break;
    case 7:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "I see.", 2);
        (*Plot)++;
        break;
    case 8:
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "Thank you, Mr. Edgeworth.\nLet's begin then.", 1);
        (*Plot)++;
        break;
    case 9:
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "You may call your\nfirst witness.", 1);
        (*Plot)++;
        break;
    case 10:
        *CurrentCharacter = Miles_Edgeworth;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution calls the\nchief officer at the scene,\nDetective Gumshoe!", 2);
        (*Plot)++;
        break;
    // end of plot
    case 11:
        *CurrentCharacter = Mia_Fey;
        MoveTile(SContext, 5, 0);
        CharacterPlayAnimation(Mia_Fey, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Mia_Fey), "Get Ready Phoenix !\nThis will be our last\nchance !", 2);
        (*Plot)++;
        break;
    case 12:
        CharacterPlayAnimation(Mia_Fey, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Mia_Fey), "On this note, this conclude this\nbrief demo.", 1);
        (*Plot)++;
        break;
    
    default:
        break;
    }
}

void CourtroomScenarioA(SceneContext* SContext, SceneContext* SEmpty, DialogueContext* DiagContext, ButtonsContext* BContext, int* Plot, int* CurrentCharacter, int* IdleAnimation, int* ReturnToDefault, char* ButtonActivated, char* buttonInput){
    switch (*Plot)
    {
    case 0:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The court is now in session for\nthe trial of Mr. Larry Butz.", 1);
        break;
    
    case 1:
        *CurrentCharacter = Winston_Payne;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The prosecution is ready,\nYour Honor.", 1);
        break;

    case 2:
        *CurrentCharacter = Phoenix_Wright;
        BackgroundPlayAnimation(SContext, 2, NULL);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The, um, defense is ready,\nYour Honor.", 1);
        break;

    case 3:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 2);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Ahem.", 1);
        break;
    case 4:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Mr. Wright?", 1);
        break;
    case 5:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "This is your first trial,\nis it not?", 1);
        break;

    case 6:
        *CurrentCharacter = Phoenix_Wright;
        MoveTile(SContext, 0, 0);
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Y-Yes, Your Honor.\nI'm, um, a little nervous.", 1);
        break;
    
    case 7:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Your conduct during this\ntrial will decide the fate\nof your client.", 1);
        break;
    case 8:
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Murder is a serious charge.\nFor your client's sake, I hope\nyou can control your nerves.", 1);
        break;

    case 9:
        *CurrentCharacter = Phoenix_Wright;
        MoveTile(SContext, 0, 0);
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Thank... thank you, Your Honor.", 1);
        break;

    case 10:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 2);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "...", 1);
        break;
    case 11:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Mr. Wright, given the\ncircumstances...", 1);
        break;
    case 12:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "I think we should have a test to\nascertain your readiness.", 1);
        break; 

    case 13:
        *CurrentCharacter = Phoenix_Wright;
        MoveTile(SContext, 0, 0);
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Yes, Your Honor.", 1);
        break;
    case 14:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 2); // Glup Animation
        SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "(Glup... Hands shaking...\nEyesight... fading...)", 1);
        break;
    
    case 15:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The test will consist of a\nfew simple questions. Answer\nthem clearly and concisely", 1);
        break;
    case 16:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Please state the name of\nthe defendant in this case.", 1);
        ClearButtons(BContext);
        AddButton(BContext, "Phoenix Wright");
        AddButton(BContext, "Larry Butz");
        AddButton(BContext, "Mia Fey");
        BackgroundPlayAnimation(SEmpty, 0, ButtonActivated);
        break;
    
    case 17:
        switch (*buttonInput)
        {
        case 0:
            *CurrentCharacter = Phoenix_Wright;
            MoveTile(SContext, 0, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Um... the defendant...\nis me, right?", 1);
            break;
        case 1:
            *CurrentCharacter = Phoenix_Wright;
            MoveTile(SContext, 0, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The defendant? Well, that's\nLarry Butz, Your Honor.", 1);
            (*Plot) = 25;
            return;
            break;
        case 2:
            *CurrentCharacter = Phoenix_Wright;
            MoveTile(SContext, 0, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The, um, defendant?\nThat's... er... Mia Fey?", 1);
            break;
        }
        break;

    case 18:
        switch (*buttonInput)
        {
        case 0:
            *CurrentCharacter = Mia_Fey;
            MoveTile(SContext, 5, 0);
            *IdleAnimation = 4;
            CharacterPlayAnimation(*CurrentCharacter, 5); // Sad
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Wright! Have you completely\nlost your mind? Focus!", 1);
            break;
        case 2:
            *CurrentCharacter = Mia_Fey;
            MoveTile(SContext, 5, 0);
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3); // Mia pose
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Wrong, Wright.\nLook, I have to leave.", 1);
            break;
        }
        break;

    case 19:
        switch (*buttonInput)
        {
        case 0:
            *IdleAnimation = 6;
            CharacterPlayAnimation(*CurrentCharacter, 7); // Shocked
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The defendant is the\nperson on trial!", 1);
            break;
        case 2:
            CharacterPlayAnimation(*CurrentCharacter, 3); // Mia Pose
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "I have to go home. I'm...\nI'm expecting a delivery.", 1);
            break;
        }
        break;
    
    case 20:
        switch (*buttonInput)
        {
        case 0:
            CharacterPlayAnimation(*CurrentCharacter, 7);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "You're his lawyer!", 1);
            break;
        case 2:
            SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "Aw, c'mon Chief.\nThere's no need to be\ngoing so soon, is there?", 1);
            break;
        }
        break;

    case 21:
        switch (*buttonInput)
        {
        case 0:
            CharacterPlayAnimation(*CurrentCharacter, 2); // Mia Pose
            SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "Um, er, eh?\nOh yeah, right! Eh heh heh.", 1);
            break;
        case 2:
            *IdleAnimation = 6;
            CharacterPlayAnimation(*CurrentCharacter, 7); // Shocked
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Wright!", 1);
            break;
        }
        break;

    case 22:
        switch (*buttonInput)
        {
        case 0:
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3); // Mia pose
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "This is no laughing matter!", 1);
            break;
        case 2:
            CharacterPlayAnimation(*CurrentCharacter, 7); // Shocked
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Listen: the defendant is\nthe one on trial--your client!", 1);
            break;
        }
        break;

    case 23:
        switch (*buttonInput)
        {
        case 0:
            CharacterPlayAnimation(*CurrentCharacter, 3); // Mia Pose
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "You did pass the bar,\ndidn't you?", 1);
            break;
        case 2:
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3); // Mia pose
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "I mean, that's about as\nbasic as you can get!", 1);
            break;
        }
        break;

    case 24:
        switch (*buttonInput)
        {
        case 0:
            *CurrentCharacter = Judge;
            MoveTile(SContext, 4, 0);
            *IdleAnimation = 0;
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Sorry, I couldn't hear your\nanswer. I'll ask once more:", 1);
            (*Plot) = 16;
            return;
            break;
        case 2:
            SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "(I put my foot in it this time!\nI've got to relax!)", 1);
            (*buttonInput) = 0;
            return;
            break;
        }
        break;

    case 25:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Correct.", 1);
        break;
    case 26:
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Just keep your wits about\nyou and you'll do fine.", 1);
        break;
    case 27:
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Next question:", 1);
        break;
    case 28:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "This is a murder trial. Tell me,\nwhat's the victim's name?", 1);
        break;

    case 29:
        *CurrentCharacter = Phoenix_Wright;
        MoveTile(SContext, 0, 0);
        *IdleAnimation = 0;
        SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "(Whew, I know this one! Glad\nI read the case report cover\nto cover so many times.)", 1);
        break;
    case 30:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 2);
        SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "(It's... wait... Uh-oh!)", 1);
        break;
    case 31:
        SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "(No.. no way! I forgot! I'm drawing a total blank here!)", 1);
        break;

    case 32:
        *CurrentCharacter = Mia_Fey;
        MoveTile(SContext, 5, 0);
        *IdleAnimation = 6;
        CharacterPlayAnimation(*CurrentCharacter, 7); // Shocked
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Phoenix! Are you absolutely\nSURE you're up to this ?", 1);
        break;
    case 33:
        CharacterPlayAnimation(*CurrentCharacter, 7); // Shocked
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "You don't even know the\nvictim's name!?", 1);
        break;
    case 34:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 2); // Mia Pose
        SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "Oh, the victim! O-Of course\nI know the victim's name!", 1);
        break;
    case 35:
        SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "I, um, just forgot.\n... Temporarily.", 1);
        break;
    case 36:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3); // Shocked
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "I think I feel a migraine\ncoming on.", 1);
        break;
    case 37:
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Look, the victim's name\nis listed in the Court Record.", 1);
        break;
    case 38:
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Just press the Court Record\nbutton to check it at anytime,\nokay?", 1);
        break;
    case 39:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3); // Mia pose
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Remember to check it often.\nDo it for me, please.\nI'm begging you.", 1);
        break;

    case 40:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Let's hear your answer.\nWho is the victim in this case?", 1);
        ClearButtons(BContext);
        AddButton(BContext, "Mia Fey");
        AddButton(BContext, "Cinder Block");
        AddButton(BContext, "Cindy Stone");
        BackgroundPlayAnimation(SEmpty, 0, ButtonActivated);
        break;

    case 41:
        switch (*buttonInput)
        {
        case 0:
            *CurrentCharacter = Phoenix_Wright;
            MoveTile(SContext, 0, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Um.. Mia Fey", 1);
            break;
        case 1:
            *CurrentCharacter = Phoenix_Wright;
            MoveTile(SContext, 0, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Oh, um, wasn't it Ms. Block?\nMs. Cinder Block?", 1);
            break;
        case 2:
            *CurrentCharacter = Phoenix_Wright;
            MoveTile(SContext, 0, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Um... the victim's name\nis Cindy Stone.", 1);
            (*Plot) = 46;
            return;
            break;
        }
        break;

    case 42:
        switch (*buttonInput)
        {
        case 0:
            *CurrentCharacter = Mia_Fey;
            MoveTile(SContext, 5, 0);
            *IdleAnimation = 6;
            CharacterPlayAnimation(*CurrentCharacter, 7);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "W-W-What!? How can I be\nthe victim!?", 1);
            break;
        case 1:
            *CurrentCharacter = Judge;
            MoveTile(SContext, 4, 0);
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The person in question was a\nvictim of murder, not ill-\nconceived naming, Mr. Wright.", 1);
            break;
        }
        break;

    case 43:
        switch (*buttonInput)
        {
        case 0:
            CharacterPlayAnimation(*CurrentCharacter, 2);
            SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "Oh! Right! Sorry! I, er,\nit was the first name that\npopped into my head, and--", 1);
            break;
        case 1:
            *CurrentCharacter = Mia_Fey;
            MoveTile(SContext, 5, 0);
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3); // Mia Pose
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Wright?", 1);
            break;
        }
        break;
    
    case 44:
        switch (*buttonInput)
        {
        case 0:
            *CurrentCharacter = Mia_Fey;
            MoveTile(SContext, 5, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The Court Record button!\nRemember to use it\nwhen you are in a pinch.", 1);
            break;
        case 1:
            CharacterPlayAnimation(*CurrentCharacter, 3); // Mia Pose
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "If you forget something, just\npress the Court Record button\nto help you remember.", 1);
            break;
        }
        break;
    
    case 45:
        switch (*buttonInput)
        {
        case 0:
            *CurrentCharacter = Judge;
            MoveTile(SContext, 4, 0);
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Let me ask that one again:", 1);
            (*Plot) = 40;
            return;
            break;
        case 1:
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "A mistake in court\ncould cost you the case.", 1);
            break;
        }
        break;

    case 46:
        switch (*buttonInput)
        {
        case 1:
            *CurrentCharacter = Judge;
            MoveTile(SContext, 4, 0);
            *IdleAnimation = 2;
            CharacterPlayAnimation(*CurrentCharacter, 3);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "I'll ask you again:", 1);
            (*Plot) = 40;
            return;
            break;
        case 2:
            *CurrentCharacter = Judge;
            MoveTile(SContext, 4, 0);
            *IdleAnimation = 0;
            CharacterPlayAnimation(*CurrentCharacter, 1);
            *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Correct.", 1);
            break;
        }
        break;

    case 47:
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "This will conclude this\nshort interaction DEMO.", 1);
        break;
    case 48:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The plot in this DEMO\nwas taken from the fist\ncase of the game.", 1);
        break;
    case 49:
        *IdleAnimation = 2;
        CharacterPlayAnimation(*CurrentCharacter, 3);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Though our skilled writers\nhave already made some progress\non Mia's legacy's plot.", 1);
        break;
    case 50:
        *IdleAnimation = 0;
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Thanks for playing.", 1);
        break;
    }
    (*Plot)++;
}

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context){

    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;
    SceneContext* SContext;
    CharacterLayer* CharaLayer;

    SceneContext* SEmpty;
    ButtonsContext* BContext;

    SDL_Rect ButtonsRect;

    int CurrentCharacter;
    int IdleAnimation;
    bool SceneFlip = false;
    int Plot = 0;
    int ReturnToDefault;
    char ButtonActivated;
    char ButtonInput;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
        return -1;
    }
    DiagContext = InitDialog(DDevice, Context->MainFont, Context->NameFont);
    SContext = InitScene(DDevice, S_Courtroom);

    ButtonsRect.x = 0;
    ButtonsRect.y = 0;
    ButtonsRect.w = DDevice->ScreenResolution.x;
    ButtonsRect.h = DDevice->ScreenResolution.y - (DiagContext->TextBounds.h + DiagContext->NameBounds.h);

    SEmpty = InitScene(DDevice, S_Empty);
    BContext = InitButtons(DDevice, SEmpty, Context->ButtonFont, &ButtonsRect);
    MoveButtonsToCoordinates(BContext, DDevice->ScreenResolution.x, 0);

    InitCharacter(DDevice, Phoenix_Wright); // Initialise the character in memory
    InitCharacter(DDevice, Mia_Fey);
    InitCharacter(DDevice, Winston_Payne);
    InitCharacter(DDevice, Judge);

    InitCharacter(DDevice, Court_Desk);

    CharaLayer = NULL;
    InitCharacterLayer(&CharaLayer, SContext);
    AddCharacterToLayer(CharaLayer, Phoenix_Wright, SContext, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, SContext, 0, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Mia_Fey, SContext, 5, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Winston_Payne, SContext, 1, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, SContext, 1, 1, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Judge, SContext, 4, 0, DDevice, SContext->SurfaceBounds);
    PlayTrackID(TRK_Courtroom);

    ButtonActivated = 0;

    // Scene setup
    CourtroomScenarioA(SContext, SEmpty, DiagContext, BContext, &Plot, &CurrentCharacter, &IdleAnimation, &ReturnToDefault, &ButtonActivated, &ButtonInput);
    // Main Loop
    while (1){
        // Events Loop
        while(SDL_PollEvent(&event)){
            if (ButtonActivated)
                HandleButtonsEvents(BContext, &event);
            switch (event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;

            case SDL_KEYDOWN:
                switch (PADKEY)
                {
                case PAD_A:
                    if (ButtonActivated) {
                        ButtonInput = GetClkdButtonID(BContext);
                        ButtonActivated = 0;
                        //printf("Reeee : %d | %d\n", SEmpty->ScenesCoordinates, SEmpty->ScenesCoordinates);
                        MoveTile(SEmpty, 0, 0);
                    }
                    CharacterPlayAnimation(CurrentCharacter, IdleAnimation); // Mouaif
                    CourtroomScenarioA(SContext, SEmpty, DiagContext, BContext, &Plot, &CurrentCharacter, &IdleAnimation, &ReturnToDefault, &ButtonActivated, &ButtonInput);
                    break;
                
                default:
                    break;
                }

                break;
            default:
                break;
            }
        }

        // Logic
        if ((DiagContext->progress >= ReturnToDefault) && (ReturnToDefault != -1)){
            CharacterPlayAnimation(CurrentCharacter, IdleAnimation);
            ReturnToDefault = -1;
        }
        // Rendering
        #ifdef _SDL
            DisplayBackground(DDevice, SContext); // Background
            DisplayCharacterLayer(DDevice, CharaLayer);
            //FlipBlitSurface(Desk, NULL, DDevice->Screen, &DeskRect, SceneFlip); // Desk
            Dialogue(IDevice, DiagContext); // Dialog
            SDL_Flip(DDevice->Screen);
        #else
            DisplayBackground(DDevice, SContext); // Background
            DisplayCharacterLayer(DDevice, CharaLayer);
            Dialogue(IDevice, DiagContext); // Dialog

            DisplayBackground(DDevice, SEmpty);
            DrawButtons(BContext);
            SDL_RenderPresent(DDevice->Renderer);
        #endif
    }


Exit:
    // Cleaning memory
/*
    #ifdef _SDL
    SDL_FreeSurface(Courtroom);
    #else
    SDL_DestroyTexture(Courtroom);
    #endif
*/
    //free(DiagContext);

    return 0;
}