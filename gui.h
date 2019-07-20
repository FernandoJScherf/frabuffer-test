#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "shapes.h"

typedef struct Button {
    uint16_t x, y;
    uint16_t w, h;
    uint32_t color;
    char* text;     //string displayed in button. Memory should be allocated in
                    //CreateButton and EventButton. Don't change this directly!!!
    //Functions that EventButton calls when certain event took place:
    void (*WhenClicked)();
    //Start with only that function, but later implement whenUnClicked and WhenPassedOver.
} Button;

Button* CreateTextButton(const char* text, uint16_t x, uint16_t y,
                        uint16_t w, uint16_t h, uint32_t color);
void ChangeTextOfButton(Button* button, const char* text);
void EventButton(Button* button, SDL_Event* e, void (*WhenClicked)());
void DrawButton(Button* button, SDL_Surface* surface);
void DestroyButton(Button* button);

#endif // GUI_H_INCLUDED
