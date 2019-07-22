#include "gui.h"
#include <string.h> //strlen

Button* CreateTextButton(const char* text, TTF_Font* textFont, uint16_t x, uint16_t y,
                        uint16_t w, uint16_t h, uint32_t color, SDL_Surface* destSurface) //Add text color.
{
    Button* button = (Button*) malloc(sizeof(Button));
    //button->text = (char*) malloc(strlen(text) + 1);

    button->x = x;    button->y = y;
    button->w = w;    button->h = h;
    //button->color = color;
//    button->whereToDrawSurface = surface;

    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(textFont, text, (SDL_Color){255, 255, 255,255}, w);
//    SDL_Surface* tempTextSurface = TTF_RenderText_Solid(textFont, text, (SDL_Color){255, 255, 255,255});
//    button->textSurface = SDL_ConvertSurface(tempTextSurface, surface->format, 0);
//    SDL_FreeSurface(tempTextSurface);

    //Final surface to draw each frame:
    //SDL_Rect rect = {x, y, w, h};

    button->drawThisSurface = SDL_CreateRGBSurface(0, w, h, destSurface->format->BitsPerPixel,
                                        destSurface->format->Rmask, destSurface->format->Gmask,
                                        destSurface->format->Bmask, destSurface->format->Amask);

    //SDL_Surface* textSurfaceOptimized = SDL_ConvertSurface(textSurface, button->drawThisSurface->format, 0);

    SDL_FillRect(button->drawThisSurface, NULL, color);

    //SDL_Rect srcRect = {0, 0, textSurface->w, textSurface->h};
    SDL_Rect dstRect = {(w - textSurface->w) / 2, (h - textSurface->h) / 2, textSurface->w, textSurface->h};

    //SDL_BlitScaled(textSurfaceOptimized, NULL, button->drawThisSurface, NULL);
    SDL_BlitSurface(textSurface, NULL, button->drawThisSurface, &dstRect);
    SDL_FreeSurface(textSurface);
    //SDL_FreeSurface(textSurfaceOptimized);
    //printf("%d - %s\n", button->drawThisSurface    , SDL_GetError()    );
    return button;
}
void ChangeTextOfButton(Button* button, TTF_Font* textFont, const char* text)       //Add text color.
{
    //button->text = (char*) realloc(button->text, strlen(text) + 1);
//    button->textSurface = TTF_RenderText_Solid(textFont, text, (SDL_Color){255, 255, 255, 255});
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


void DrawButton(Button* button, SDL_Surface* destSurface)
{
    SDL_Rect destRect = {button->x, button->y, button->w, button->h};
    SDL_BlitSurface(button->drawThisSurface, NULL, destSurface, &destRect);
//    printf("%d - %s\n", SDL_BlitSurface(button->drawThisSurface, NULL, destSurface, NULL)
//    , SDL_GetError()
//    );
}
void DestroyButton(Button* button)
{
    SDL_FreeSurface(button->drawThisSurface);//    SDL_FreeSurface(button->textSurface); //free(button->text);
    free(button);
}















