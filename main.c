#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h> //rand()

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
    int16_t toReturn = 0;   //Will keep being 0 if everything worked fine. Will change in case of critical errors.

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//SDL_Texture* screenTexture = NULL; //Later I will test it with a texture that will be rendered to the screen instead of with a surfac. For now let-s keep it simple.

	//Initialize SDL. VIDEO and TIMER
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{
		printf( "SDL_Init failed. SDL_GetError(): %s\n", SDL_GetError() );
		toReturn = -1;
	}
	else
	{
        //Create window
		window = SDL_CreateWindow( "SDL testing framebuffer technique", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

		if( window == NULL )
		{
			printf( "SDL_CreateWindow failed. SDL_Error: %s\n", SDL_GetError() );
			toReturn = -2;
		}
		else if( TTF_Init() == -1 )
        {
            printf( "TTF_Init() failed. TTF_GetError(): %s\n", TTF_GetError() );
            toReturn = -3;
        }
        else
		{
            TTF_Font *textFont = TTF_OpenFont("synchronizer_nbp.ttf", 16);
            if(!textFont)
            {
                printf("TTF_OpenFont failed. TTF_GetError(): %s", TTF_GetError());
                toReturn = -4;
            }

			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

			uint8_t r, g, b = 0;
			uint32_t *pixels = (uint32_t *)screenSurface->pixels;

			uint32_t currentTime = 0;
			uint32_t lastTime = 0;
			uint32_t frameTime = 0;

			//string to hold the frameTime and atter draw it to the screen through textSurface:
			char frameTimeString[100];      //Hardcoding is bad, kids.

			SDL_Color textColor = {255,255,255};
			SDL_Surface *textSurface = NULL;

			int pixelsQuantity = screenSurface->w * screenSurface->h;   //Total number of pixels of the screen surface, used in the loop to fill it.

			while(1)
            {
                lastTime = currentTime;
                currentTime = SDL_GetTicks();
                frameTime = currentTime - lastTime;

                sprintf(frameTimeString, "frame Time: %d", frameTime);

                SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 255, 255));

                //Playing with the PIXELS of the surface: /////////////////////////////////////////////////
                SDL_LockSurface(screenSurface);
//
//                r = rand() % 256;
//                g = rand() % 256;
//                b = rand() % 256;
//
                for(int i = 0; i < pixelsQuantity; i++)
                {
                    pixels[i] = 0;//SDL_MapRGB( screenSurface->format, r, g, b);
                    //Tiene que haber una forma de evitar el SDL_MapRGB. Mir'a lo que dice la migration guide de hacer
                    //que pixels haya sido malloqueado y cada uno es simplemente un int32.
                    //Este llamado a SDL por cada pixel debe estar comiendo muchisimo tiempo.
                    //SACALO. Creo que en If your game just wants to get fully-rendered frames to the screen
                    //puede estar la clave.
                    //Una vez que consiga los 16 milisegundos por frame, dibujando cada pixel, puedo comenzar a implementar
                    //los poligonos / triangulos. Cu'al ser'a m'as eficiente de los dos?
                    // https://wiki.libsdl.org/MigrationGuide
                    // https://wiki.libsdl.org/SDL_Surface?highlight=%28%5CbCategoryStruct%5Cb%29%7C%28CategorySurface%29
                    // https://alienryderflex.com/polygon_fill/
                    // http://alienryderflex.com/polygon/
                }

                SDL_UnlockSurface(screenSurface);
                ///////////////////////////////////////////////////////////////////////////////////////////

                textSurface = TTF_RenderText_Solid(textFont, frameTimeString, textColor);
                if(!textSurface)
                {
                    printf("TTF_RenderText_Solid failed. TTF_GetError(): %s", TTF_GetError());
                    toReturn = -5;
                }

                SDL_BlitSurface(textSurface, NULL, screenSurface, NULL);

                //Update the surface
                SDL_UpdateWindowSurface( window );
            }

			//Wait two seconds
			SDL_Delay( 20000 );
		}
	}

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	printf("Out of main function! Bye! Thank you for playing!\n");
	return toReturn;
}
