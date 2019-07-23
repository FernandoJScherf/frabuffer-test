#ifndef EDITOR_GUI_OBJECTS_H_INCLUDED
#define EDITOR_GUI_OBJECTS_H_INCLUDED
//Let's build a simple gui with what I implemented in gui.h and gui.c:
//I will implement everything here just so I can keep main clean and tidy.

#include "gui.h"

SDL_Surface* editorSurface = NULL;

TTF_Font *editorFont = NULL;

typedef enum States {   None, Start_New_Triangle, Add_New_Point_To_Triangle, Finish_Triangle,
                        Start_New_Poly  } States;

States editorState = None;

typedef struct Triangle {
    Point v[3];            //Array of three points;
    int32_t color;
} Triangle;

Triangle** arrayTri = NULL;   //Array of pointers to triangles elements/objects.
int16_t arrayTriSize = 0;

//Functions: ////////////////////////////////////////////////////////////////////////
void test1()
{
    printf("click1. \n");
    editorState = Start_New_Triangle;
}
void test2()
{
    printf("click2. \n");
    editorState = Start_New_Poly;
}

void Editor_Init(SDL_Surface* surface)
{
    editorFont = TTF_OpenFont("synchronizer_nbp.ttf", 8); //Check for error.
    if(editorFont == NULL)
        printf("TTF_OpenFont failed! Probably the font is missiong! TTF_GetError: %s\n", TTF_GetError());

    editorSurface = surface;

    GUI_Init(editorSurface);

    GUI_CreateTextButton(   test1, "go to test1", editorFont,
                            editorSurface->w - 50, 10, 40, 20, 0xFF000000);
    GUI_CreateTextButton(   test2, "go to test2", editorFont,
                            editorSurface->w - 50, 40, 40, 20, 0xFF000000);
}

void Editor_Quit()
{
    SDL_FreeSurface(editorSurface);
    TTF_CloseFont(editorFont);
    GUI_Quit();
}

void Editor_EventsHandler(SDL_Event* e)
{
    if(GUI_EventButtons(e))        //If a click was registered on any of the gui elements, return.
        return;
    //If we continue, it means the click was on a drawable part of the surface:
    int x, y;
    SDL_GetMouseState( &x, &y );

    static int16_t nPoint = 0;

    switch(editorState)
    {
        case None:

            break;
        case Start_New_Triangle:
            //Get mouse position
            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                //Make the array bigger to hold the new pointer to a triangle:
                arrayTri = (Triangle*)realloc(arrayTri, (arrayTriSize + 1) * sizeof(Triangle*) );
                //Add one triangle to the array:
                arrayTri[arrayTriSize] = (Triangle*) malloc(sizeof(Triangle));

                editorState = Add_New_Point_To_Triangle;
                nPoint = 0;
            }
            //break; Without the break it adds already the first point!
        case Add_New_Point_To_Triangle:
            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                arrayTri[arrayTriSize]->v[nPoint].x = x;
                arrayTri[arrayTriSize]->v[nPoint].y = y;
                printf("Point %d for triangle at x %f y %f \n", nPoint, arrayTri[arrayTriSize]->v[nPoint].x, arrayTri[arrayTriSize]->v[nPoint].y);
                printf("Point %d for triangle at x %d y %d \n", nPoint, x, y);
                if(++nPoint == 3)
                {
                    editorState = Start_New_Triangle;
                    arrayTriSize++;

                }

            }
            break;
        case Finish_Triangle:
            editorState = Start_New_Triangle;


            break;


        case Start_New_Poly:
            if(e->type == SDL_MOUSEBUTTONDOWN)
                printf("New point for poly at x %d y %d \n", x, y);
            break;
    }

}

void Editor_Draw()
{
    GUI_DrawButtons();
    //if(arrayTriSize > 0)
    for(int i = 0; i < arrayTriSize; i++)
        TriangleFlat(   arrayTri[i]->v[0], arrayTri[i]->v[1], arrayTri[i]->v[2],
                        0xFFFFFFFF, editorSurface);
}

void Editor_Update()
{
//    switch(editorState)
//    {
//        case None:
//            //printf("Updating None State.\n");
//            break;
//        case Drawing_Triangle:
//            //printf("Updating Drawing Triangle State.\n");
//            break;
//        case Drawing_Poly:
//            //printf("Updating Drawing Poly State.\n");
//            break;
//    }
}


#endif // EDITOR_GUI_OBJECTS_H_INCLUDED
