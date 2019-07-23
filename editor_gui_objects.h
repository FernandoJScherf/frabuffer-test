#ifndef EDITOR_GUI_OBJECTS_H_INCLUDED
#define EDITOR_GUI_OBJECTS_H_INCLUDED
//Let's build a simple gui with what I implemented in gui.h and gui.c:
//I will implement everything here just so I can keep main clean and tidy.

#include "gui.h"

SDL_Surface* editorSurface = NULL;

TTF_Font *editorFont = NULL;

typedef enum States {   None, Start_New_Triangle, Add_New_Point_To_Triangle,
                        Start_New_Poly, Add_New_Point_To_Poly  } States;

States editorState = None;

typedef struct Triangle {
    Point v[3];            //Array of three points;
    int32_t color;
} Triangle;

Triangle** arrayTri = NULL;   //Array of pointers to triangles elements/objects.
int16_t arrayTriSize = 0;

typedef struct Poly {
    Triangle** t;        //A pointer to a dynamic array of pointers to triangles (That will be in the array of triangles).
    int16_t tSize;      //The size of the array of triangles.
    int32_t color;      //The color of the polygon.
} Poly;

Poly** arrayPoly = NULL;
int16_t arrayPolySize = 0;

//Button's Functions: //////////////////////////////////////////////////////////////////////
void test1()
{
//Probably the simplest solution would be to implement here specific logic to handle
//From what state you are entering and to what step you are going. This buttons could
//Prepare everything so that later we could create triangles and polygons
//without problems.
    printf("click1. \n");
    editorState = Add_New_Point_To_Triangle;
}
void test2()
{
    printf("click2. \n");
    editorState = Add_New_Point_To_Poly;
}
////////////////////////////////////////////////////////////////////////////////////////////

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
    if(GUI_EventButtons(e))        //If a click was registered on any of the gui elements, return;
        return;
    //If we continue, it means the click was on a drawable part of the surface:

    //Get mouse position:
    int x, y;
    SDL_GetMouseState( &x, &y );

    static int16_t nPoint = 0;

    switch(editorState)
    {
        case None:

            break;
        case Add_New_Point_To_Triangle:

            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                if(nPoint == 0)
                {
                    //If it's the first vertex of the triangle, we need to allocate memory:
                    //Make the array bigger to hold the new pointer to a triangle:
                    arrayTri = (Triangle*)realloc(arrayTri, (arrayTriSize + 1) * sizeof(Triangle*) );
                    //Add one triangle to the array:
                    arrayTri[arrayTriSize] = (Triangle*) malloc(sizeof(Triangle));
                }


                //Then we specify the point:
                arrayTri[arrayTriSize]->v[nPoint].x = x;
                arrayTri[arrayTriSize]->v[nPoint].y = y;

                printf("Point %d for triangle at x %d y %d \n", nPoint, x, y);
                printf("arrayTri[arrayTriSize] .. arrayTri[%d] ==  %d \n", arrayTriSize, arrayTri[arrayTriSize]);

                //If it was the last one, the next click has to start again at point 0.
                if(++nPoint == 3)
                {
                    //editorState = Start_New_Triangle;
                    arrayTriSize++;
                    nPoint = 0;
                }

            }
            break;

        case Add_New_Point_To_Poly:

            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                if(nPoint == 0 || nPoint >= 3)
                {
                    //If it's the first vertex of the triangle, we need to allocate memory:
                    //Make the array bigger to hold the new pointer to a triangle:
                    arrayTri = (Triangle*)realloc(arrayTri, (arrayTriSize + 1) * sizeof(Triangle*) );
                    //Add one triangle to the array:
                    arrayTri[arrayTriSize] = (Triangle*) malloc(sizeof(Triangle));

                }

                if(nPoint < 3)
                {
                    //We specify the new point:
                    arrayTri[arrayTriSize]->v[nPoint].x = x;
                    arrayTri[arrayTriSize]->v[nPoint].y = y;
                }
                else
                {
                    //We specify the points that we already know:
                    arrayTri[arrayTriSize]->v[0].x = arrayTri[arrayTriSize-1]->v[1].x;
                    arrayTri[arrayTriSize]->v[0].y = arrayTri[arrayTriSize-1]->v[1].y;
                    arrayTri[arrayTriSize]->v[1].x = arrayTri[arrayTriSize-1]->v[2].x;
                    arrayTri[arrayTriSize]->v[1].y = arrayTri[arrayTriSize-1]->v[2].y;
                    //We specify the new point:
                    arrayTri[arrayTriSize]->v[2].x = x;
                    arrayTri[arrayTriSize]->v[2].y = y;
                }

                printf("Point %d for triangle at x %d y %d \n", nPoint, x, y);
                printf("arrayTri[arrayTriSize] .. arrayTri[%d] ==  %d \n", arrayTriSize, arrayTri[arrayTriSize]);

                //If it was the last one, the next click has to create a new triangle
                //Starting with the last two vertices:
                if(++nPoint >= 3)
                {
                    //If it's the first vertex of new triangle, we need to allocate memory:
                    arrayTriSize++;

                }

            }
            break;  //When we are drawing a triangle and then change to a poly, or vice-versa, the triangle
                    //starts being drawn again, and the pointer to the triangle that was being drawn before
                    //GETS LOTS! MEMORY LEAK!
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
