#include "gui.h"
#include <string.h> //strlen

Button** arrayGui = NULL;   //Array of pointers to gui elements/objects.
int16_t arrayGuiSize = 0;

//GUI_Init()
//{
//
//}

void CreateTextButton(  void (*WhenClicked)(),
                        const char* text, TTF_Font* textFont, uint16_t x, uint16_t y,
                        uint16_t w, uint16_t h, uint32_t color, SDL_Surface* destSurface) //Add text color.
{
    //Make the array bigger to hold the gui element created.
    arrayGui = (Button*)realloc(arrayGui, (arrayGuiSize + 1) * sizeof(Button*) );
    //Add one button to the array.
    *(arrayGui + arrayGuiSize) = (Button*) malloc(sizeof(Button));

    //button->text = (char*) malloc(strlen(text) + 1);

    arrayGui[arrayGuiSize]->x = x;    arrayGui[arrayGuiSize]->y = y;
    arrayGui[arrayGuiSize]->w = w;    arrayGui[arrayGuiSize]->h = h;
    arrayGui[arrayGuiSize]->WhenClicked = WhenClicked;
    //button->color = color;
//    button->whereToDrawSurface = surface;

    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(textFont, text, (SDL_Color){255, 255, 255,255}, w);
//    SDL_Surface* tempTextSurface = TTF_RenderText_Solid(textFont, text, (SDL_Color){255, 255, 255,255});
//    button->textSurface = SDL_ConvertSurface(tempTextSurface, surface->format, 0);
//    SDL_FreeSurface(tempTextSurface);

    //Final surface to draw each frame:
    //SDL_Rect rect = {x, y, w, h};

    arrayGui[arrayGuiSize]->drawThisSurface = SDL_CreateRGBSurface(0, w, h, destSurface->format->BitsPerPixel,
                                        destSurface->format->Rmask, destSurface->format->Gmask,
                                        destSurface->format->Bmask, destSurface->format->Amask);

    //SDL_Surface* textSurfaceOptimized = SDL_ConvertSurface(textSurface, button->drawThisSurface->format, 0);

    SDL_FillRect(arrayGui[arrayGuiSize]->drawThisSurface, NULL, color);

    //SDL_Rect srcRect = {0, 0, textSurface->w, textSurface->h};
    SDL_Rect dstRect = {    (w - arrayGui[arrayGuiSize]->w) / 2, (h - textSurface->h) / 2,
                            textSurface->w, textSurface->h};

    //SDL_BlitScaled(textSurfaceOptimized, NULL, button->drawThisSurface, NULL);
    SDL_BlitSurface(textSurface, NULL, arrayGui[arrayGuiSize]->drawThisSurface, &dstRect);
    SDL_FreeSurface(textSurface);
    //SDL_FreeSurface(textSurfaceOptimized);
    //printf("%d - %s\n", button->drawThisSurface    , SDL_GetError()    );
    //return button;
    arrayGuiSize++;
}
void ChangeTextOfButton(Button* button, TTF_Font* textFont, const char* text)       //Add text color.
{
    //button->text = (char*) realloc(button->text, strlen(text) + 1);
//    button->textSurface = TTF_RenderText_Solid(textFont, text, (SDL_Color){255, 255, 255, 255});
}
void EventButtons(SDL_Event* e)   //Eliminate the second parameter. That function should be specified inside of the button struct.
{
     //Get mouse position
     int x, y;
     SDL_GetMouseState( &x, &y );

    for(int i = 0; i < arrayGuiSize; i++)
    {
         if(e->type == SDL_MOUSEBUTTONDOWN &&
                x < (arrayGui[i]->w + arrayGui[i]->x) && x > arrayGui[i]->x &&
                y < (arrayGui[i]->h + arrayGui[i]->y) && y > arrayGui[i]->y)
            arrayGui[i]->WhenClicked();
    }

}


void DrawButtons(SDL_Surface* destSurface)
{
    SDL_Rect destRect;
    for(int i = 0; i < arrayGuiSize; i++)
    {
        destRect.x = arrayGui[i]->x; destRect.y = arrayGui[i]->y;
        destRect.w = arrayGui[i]->w; destRect.h = arrayGui[i]->h;
        SDL_BlitSurface(arrayGui[i]->drawThisSurface, NULL, destSurface, &destRect);
    }

//    printf("%d - %s\n", SDL_BlitSurface(button->drawThisSurface, NULL, destSurface, NULL)
//    , SDL_GetError()
//    );
}
void DestroyButton(Button* button)
{
    SDL_FreeSurface(button->drawThisSurface);//    SDL_FreeSurface(button->textSurface); //free(button->text);
    free(button);
}













