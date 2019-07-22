#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h> //rand()
#include "shapes.h"
#include "gui.h"

// https://wiki.libsdl.org/MigrationGuide
// https://wiki.libsdl.org/SDL_Surface
// https://alienryderflex.com/polygon_fill/
// http://alienryderflex.com/polygon/

//Some globlals:
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BIT_DEPTH = 32;

SDL_Window* window = NULL;
SDL_Renderer* sdlRenderer = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Texture* sdlTexture = NULL;

TTF_Font *textFont = NULL;
TTF_Font *buttonFont = NULL;

//The init and cleanAndClose functions, just to keep main a little bit more organized:
void cleanAndClose()
{
    TTF_CloseFont(textFont);
    TTF_CloseFont(buttonFont);
    TTF_Quit();

    SDL_FreeSurface(screenSurface);
    SDL_DestroyRenderer(sdlRenderer);   //destroy the rendering context for a window and free associated textures. (So it's not necessary to free the individual sdlTexture).
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Everything clean and ready to close!\n");
}

char init()
{
    //Initialize SDL. VIDEO and TIMER:
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0) //Check for error.
    {
        printf("SDL_Init failed! SDL_GetError: %s\n: ", SDL_GetError());
        return 1;
    }
    printf("SDL Initialized without problems!\n");

    ////
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("We compiled against SDL version %d.%d.%d ...\n",
           compiled.major, compiled.minor, compiled.patch);
    printf("But we are linking against SDL version %d.%d.%d.\n",
           linked.major, linked.minor, linked.patch);
    ////

    //Create the window we'll be rendering to:
	window = SDL_CreateWindow( "Graphics Editor 0.1",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED );
    if(window == NULL)  //Check for error.
    {
        printf("SDL_CreateWindow failed! SDL_GetError: %s\n: ", SDL_GetError());
        return 2;
    }
    printf("Window created without problems!\n");

    //Create the rendering context:
	sdlRenderer = SDL_CreateRenderer(window, -1, 0);
	if(sdlRenderer == NULL) //Check for error.
    {
        printf("SDL_CreateRenderer failed! SDL_GetError: %s\n: ", SDL_GetError());
        return 3;
    }
    printf("Renderer created without problems!\n");

    //Create the surface where we will render everything:
    screenSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                        SCREEN_BIT_DEPTH,
                                        0x00FF0000, 0x0000FF00,
                                        0x000000FF, 0xFF000000);
	if(screenSurface == NULL) //Check for error.
    {
        printf("SDL_CreateRGBSurface failed! SDL_GetError: %s\n: ", SDL_GetError());
        return 4;
    }
    printf("Screen's surface created without problems!\n");

    //Create the texture where we will copy the surface every frame to scale to full screen:
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        SCREEN_WIDTH, SCREEN_HEIGHT);
	if(sdlTexture == NULL) //Check for error.
    {
        printf("SDL_CreateTexture failed! SDL_GetError: %s\n: ", SDL_GetError());
        return 5;
    }
    printf("Screen's texture created without problems!\n");

    if(TTF_Init() < 0) //Check for error.
    {
        printf("TTF_Init failed! TTF_GetError: %s\n: ", TTF_GetError());
        return 6;
    }

    textFont = TTF_OpenFont("synchronizer_nbp.ttf", 16); //Check for error.
    buttonFont = TTF_OpenFont("synchronizer_nbp.ttf", 8); //Check for error.

    if(textFont == NULL || buttonFont == NULL)
    {
        printf("TTF_OpenFont failed! Probably the font is missiong! TTF_GetError: %s\n", TTF_GetError());
        return 7;
    }

    printf("Font subsystem initializated without problems and fonts loaded!\n");
    printf("Initialization ended without problems!\n");
    return 0;
}

uint32_t triangleColor = 0xFF000000;

void testFunctionForClickedButton()
{
    static int click = 0;
    printf("click %d!\n", click);
    click++;
    triangleColor = rand();
    printf("El color al azar es:  %d!\n", triangleColor);
}

void testFunction1()
{
    testFunctionForClickedButton();
    printf("This time the first button was pressed!\n");
}

void testFunction2()
{
    testFunctionForClickedButton();
    printf("This time the second button was pressed!\n");
}

void testFunction3()
{
    testFunctionForClickedButton();
    printf("This time the third button was pressed!\n");
}

int main( int argc, char* args[] )
{
    //Start up SDL and create window, surface to draw pixels, and texture where to copy the
    //surface and render to the screen:
    if(init() == 0) //If everything went well:
    {
        //We will need them to update the screen sdlTexture later:
        uint32_t *pixels = screenSurface->pixels;
        uint32_t pitch = screenSurface->pitch;

        SDL_Color textColor = {255,255,255};
        SDL_Surface *textSurface = NULL;

        //string to hold the frameTime and latter draw it to the screen through textSurface:
        char frameTimeString[100];      //Hardcoding is bad, kids.

        uint32_t lastTime = 0;
        uint32_t frameTime = 0;
        uint32_t maxTime = 0;   //The maximum time that we waited updating and rendering.
        uint32_t currentTime = SDL_GetTicks();

        //A triangle:
        Point v1;        v1.x = 100;       v1.y = 0;
        Point v2;        v2.x = 100;       v2.y = 0;
        Point v3;        v3.x = 50;        v3.y = 150;

        //A button:
        CreateTextButton(testFunction1, "Change!", buttonFont, 10, 50, 100, 30, 0xFF000000, screenSurface);
        CreateTextButton(testFunction2, "Change 2!", buttonFont, 100, 150, 100, 30, 0xFF000000, screenSurface);
        CreateTextButton(testFunction3, "Change three!", buttonFont, 150, 250, 50, 20, 0xFF000000, screenSurface);

        uint8_t keepRunning = 1;    //Main loop flag!   1 means "yes".

        SDL_Event e;                //Event handler!

        while(keepRunning)
        {
            //Calculate and print time between frames:
            lastTime = currentTime;
            currentTime = SDL_GetTicks();
            frameTime = currentTime - lastTime;

            if(frameTime > 15)
                maxTime = frameTime;

            sprintf(frameTimeString, "frame Time: %i - last bad max time: %i", frameTime, maxTime);

            //Handle events on queue:
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT)          //User requests quit.
                    keepRunning = 0;            //0 means "no".
                else if(e.type == SDL_KEYDOWN)
                {
                    switch( e.key.keysym.sym )
                    {
                        case SDLK_ESCAPE:       //User requests quit.
                            keepRunning = 0;    //0 means "no".
                            break;
                    }
                }
                else if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
                    EventButtons(&e);
            }

            //Playing with the PIXELS of the surface: /////////////////////////////////////////////////
            //Erase everything on the surface to white before drawing again:
            SDL_FillRect(screenSurface, NULL, 0xFFF000F0);

            SDL_LockSurface(screenSurface);

            v1.x = 0;//rand() % SCREEN_WIDTH;
            v1.y = SCREEN_HEIGHT;//rand() % SCREEN_HEIGHT;
            v2.x = SCREEN_WIDTH; //rand() % SCREEN_WIDTH;
            v2.y = SCREEN_HEIGHT; //rand() % SCREEN_HEIGHT;
            v3.x = SCREEN_WIDTH / 2; //rand() % SCREEN_WIDTH;
            v3.y = 0; //rand() % SCREEN_HEIGHT;

            TriangleFlat(v1, v2, v3, triangleColor, screenSurface);

            SDL_UnlockSurface(screenSurface);
            ///////////////////////////////////////////////////////////////////////////////////////////

            //Draw button:
            DrawButtons(screenSurface);


            textSurface = TTF_RenderText_Solid(textFont, frameTimeString, textColor);
            //Is this surface in a different format than screenSurface?
            //If thats the case, in each frame, a conversion occurs. Can this surface
            //just be rendered with the same format?

            SDL_BlitSurface(textSurface, NULL, screenSurface, NULL);

            SDL_UpdateTexture(sdlTexture, NULL, pixels, pitch);

            //SDL_RenderClear(sdlRenderer);

            SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
            SDL_RenderPresent(sdlRenderer);

            //SDL_Delay(100);

        }
    SDL_FreeSurface(textSurface);
    GUI_Quit();

    }

	cleanAndClose();

	printf("Out of main function! Bye! Thank you for playing!\n");
}
