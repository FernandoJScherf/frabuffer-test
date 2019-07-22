#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "shapes.h"

typedef struct GUI_Button {
    uint16_t x, y;
    uint16_t w, h;
    //uint8_t visible;
    //uint32_t color;
    //char* text;     //string displayed in button. Memory should be allocated in
                    //CreateButton and EventButton. Don't change this directly!!!
    SDL_Surface* drawThisSurface;
    //TTF_Font *textFont;
    //SDL_Color textColor;
    //Functions that EventButton calls when certain event took place:
    void (*WhenClicked)();
    //Start with only that function, but later implement whenUnClicked and WhenPassedOver.
} GUI_Button;

//Create a button and add it to the internal array of gui elements:
//If the user wants, can keep a pointer to the button created, so he can later pass it to
//other functions to modify it, like change text, or direcrly change its position.
GUI_Button* GUI_CreateTextButton(  void (*WhenClicked)(),
                            const char* text, TTF_Font* textFont, uint16_t x, uint16_t y,
                            uint16_t w, uint16_t h, uint32_t color);
//void ChangeTextOfButton(Button* button, TTF_Font* textFont, const char* text);
int8_t GUI_EventButtons(SDL_Event* e);
void GUI_DrawButtons();
void GUI_DestroyButtons();

void GUI_Init(SDL_Surface* destSurface);  //To call before using any of the other functions.
void GUI_Quit();                    //To free all memory allocated for the array of buttons.

#endif // GUI_H_INCLUDED
