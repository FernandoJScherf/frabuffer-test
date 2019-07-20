#include "gui.h"
#include <string.h> //strlen

Button* CreateTextButton(const char* text, uint16_t x, uint16_t y,
                        uint16_t w, uint16_t h, uint32_t color)
{
    Button* button = (Button*) malloc(sizeof(Button));
    button->text = (char*) malloc(strlen(text) + 1);

    button->x = x;    button->y = y;
    button->w = w;    button->h = h;
    button->color = color;

}
void ChangeTextOfButton(Button* button, const char* text)
{
    button->text = (char*) realloc(button->text, strlen(text) + 1);
}
void EventButton(Button* button, SDL_Event* e, void (*WhenClicked)())
{
     //Get mouse position
     int x, y;
     SDL_GetMouseState( &x, &y );

     if(e->type == SDL_MOUSEBUTTONDOWN &&
            x < (button->w + button->x) && x > button->x &&
            y < (button->h + button->y) && y > button->y)
        (*WhenClicked)();
}
void DrawButton(Button* button, SDL_Surface* surface)
{
        //A rectangle made of two triangles:
        Point v1;
        v1.x = button->x;               v1.y = button->y;
        Point v2;
        v2.x = button->x + button->w;   v2.y = button->y;
        Point v3;
        v3.x = button->x;               v3.y = button->y + button->h;
        TriangleFlat(v1, v2, v3, surface);
        v1.x = button->x + button->w;   v1.y = button->y + button->h;
        TriangleFlat(v3, v2, v1, surface);

        //Text:
}
void DestroyButton(Button* button)
{
    free(button->text);
    free(button);
}
