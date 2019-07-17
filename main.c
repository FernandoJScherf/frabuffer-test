#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h> //rand()
#include "shapes.h"

// https://wiki.libsdl.org/MigrationGuide
// https://wiki.libsdl.org/SDL_Surface
// https://alienryderflex.com/polygon_fill/
// http://alienryderflex.com/polygon/
// triangles:
//      http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo1
// https://www.joshbeam.com/articles/triangle_rasterization/
// mas sofisticado, con gradientes y texturas:
// http://www-users.mat.uni.torun.pl/~wrona/3d_tutor/tri_fillers.html
// http://www.salomonsson.se/devlog/math5.html
// http://www.hugi.scene.org/online/coding/hugi%2017%20-%20cotriang.htm

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BIT_DEPTH = 32;

int main( int argc, char* args[] )
{

	//Initialize SDL. VIDEO and TIMER
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ); //Check for error.

    //Create window //Check for error.
    	//The window we'll be rendering to
	SDL_Window* window = SDL_CreateWindow( "SDL testing framebuffer technique",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED );

    //The rendering context.
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, 0); //Check for error.

	TTF_Init(); //Check for error.
    TTF_Font *textFont = TTF_OpenFont("synchronizer_nbp.ttf", 16); //Check for error.

    //SDL_Surface *windowSurface = SDL_GetWindowSurface(window);

    SDL_Surface *screenSurface = SDL_CreateRGBSurface(0,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SCREEN_BIT_DEPTH,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);    //Check for error.

	SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        SCREEN_WIDTH, SCREEN_HEIGHT);

	//uint8_t r, g, b = 0;
	int pixelsQuantity = screenSurface->w * screenSurface->h;   //Total number of pixels of the screen surface, used in the loop to fill it.

	uint32_t *pixels = screenSurface->pixels;
	uint32_t pitch = screenSurface->pitch;

	//string to hold the frameTime and latter draw it to the screen through textSurface:
	char frameTimeString[100];      //Hardcoding is bad, kids.

    SDL_Color textColor = {255,255,255};
	SDL_Surface *textSurface = NULL;

	uint32_t lastTime = 0;
	uint32_t frameTime = 0;
	uint32_t maxTime = 0;   //The maximum time that we waited updating and rendering.
    uint32_t currentTime = SDL_GetTicks();

    uint32_t color = 0;

    //The bottom triangle. Requirements:    v1.y < v2.y
    //                                      v2.y == v3.y

    Vertice v1;
    v1.x = 100;
    v1.y = 10;

    Vertice v2;
    v2.x = 50;
    v2.y = 90;

    Vertice v3;
    v3.x = 150;
    v3.y = 90;

    //The top triangle. requirementes:      v3.y > v1.y
    //                                      v1.y == v2.y
    Vertice vt1;
    vt1.x = 300;
    vt1.y = 50;

    Vertice vt2;
    vt2.x = 500;
    vt2.y = 50;

    Vertice vt3;
    vt3.x = 400;
    vt3.y = 300;

    while(currentTime < 15000)
    {
        lastTime = currentTime;
        currentTime = SDL_GetTicks();
        frameTime = currentTime - lastTime;
        if(frameTime > 15)
            maxTime = frameTime;

        sprintf(frameTimeString, "frame Time: %i - last bad max time: %i", frameTime, maxTime);

        SDL_FillRect(screenSurface, NULL, 0xFFF000F0);

        //Playing with the PIXELS of the surface: /////////////////////////////////////////////////
        SDL_LockSurface(screenSurface);


        FillUpTriangle(v1, v2, v3, screenSurface);
        FillDownTriangle(vt1, vt2, vt3, screenSurface);

        v3.x++;
        if(v3.x >= SCREEN_WIDTH)
            v3.x = 150;

        v3.y = ++v2.y;
        if(v3.y >= SCREEN_HEIGHT)
            v3.y = v2.y = 90;

//        color = 0xFFF00FFF;
//
//        for(int i = 0; i < pixelsQuantity; i++)
//        {
//            pixels[i] = color;//SDL_MapRGB( screenSurface->format, r, g, b);
//            //Evitar el SDL_MapRGB.
//            //Este llamado por cada pixel debe estar comiendo muchisimo tiempo.
//            //Una vez que consiga los 16 milisegundos por frame, dibujando cada pixel, puedo comenzar a implementar
//            //los poligonos / triangulos. Cu'al ser'a m'as eficiente de los dos?
//        }

        SDL_UnlockSurface(screenSurface);
        ///////////////////////////////////////////////////////////////////////////////////////////

        textSurface = TTF_RenderText_Solid(textFont, frameTimeString, textColor);
        //Is this surface in a different format than screenSurface?
        //If thats the case, in each frame, a conversion occurs. Can this surface
        //just be rendered with the same format?
/*        if(!textSurface)
        {
            printf("TTF_RenderText_Solid failed. TTF_GetError(): %s", TTF_GetError());
            toReturn = -5;
        }
*/
        SDL_BlitSurface(textSurface, NULL, screenSurface, NULL);


        SDL_UpdateTexture(sdlTexture, NULL, pixels, pitch);

        //SDL_RenderClear(sdlRenderer);

        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
        SDL_RenderPresent(sdlRenderer);

    }

    free(pixels);

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	printf("Out of main function! Bye! Thank you for playing!\n");
}
