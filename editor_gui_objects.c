#include "editor_gui_objects.h"

//The states that the editor can be in:
typedef enum States {   None, Start_New_Triangle, Add_New_Point_To_Triangle,
                        Start_New_Poly, Add_New_Point_To_Poly  } States;

//A structure that represents a triangle:
typedef struct Triangle {
    Point v[3];            //Array of three points;
    int32_t color;
} Triangle;

//Some globals for this file:
static SDL_Surface* editorSurface = NULL;   //The surface that will be drawn by the editor.

static TTF_Font *editorFont = NULL;         //The font that will be used by the editor.

static States editorState;                  //The current state of the editor. Initialized in Editor_Init.

static Triangle** arrayTri = NULL;          //Array of pointers to triangles elements/objects.
static int16_t arrayTriLastPos = 0;        //The size of the array of triangles.

typedef struct Poly {
    Triangle** t;        //A pointer to a dynamic array of pointers to triangles (That will be in the array of triangles).
    int16_t tSize;      //The size of the array of triangles.
    int32_t color;      //The color of the polygon.
} Poly;

static Poly** arrayPoly = NULL;
static int16_t arrayPolySize = 0;

//Button's Functions: //////////////////////////////////////////////////////////////////////
static void test1()
{
    printf("click1. \n");
    //editorState = Add_New_Point_To_Triangle;
    Editor_Change_State(Add_New_Point_To_Triangle);
}
static void test2()
{
    printf("click2. \n");
    //editorState = Add_New_Point_To_Poly;
    Editor_Change_State(Add_New_Point_To_Poly);
}
////////////////////////////////////////////////////////////////////////////////////////////


void Editor_Init(SDL_Surface* surface)
{
    editorFont = TTF_OpenFont("synchronizer_nbp.ttf", 8); //Check for error.
    if(editorFont == NULL)
        printf("TTF_OpenFont failed! Probably the font is missiong! TTF_GetError: %s\n", TTF_GetError());

    editorSurface = surface;

    editorState = None;

    GUI_Init(editorSurface);

    GUI_CreateTextButton(   test1, "go to test1", editorFont,
                            editorSurface->w - 50, 10, 40, 20, 0xFF000000);
    GUI_CreateTextButton(   test2, "go to test2", editorFont,
                            editorSurface->w - 50, 40, 40, 20, 0xFF000000);
}

void Editor_Quit()
{
    //Put everything that should be NULLed here.
    SDL_FreeSurface(editorSurface);
    TTF_CloseFont(editorFont);
    GUI_Quit();
}

//Some helper functions: ////////////////////////////////////
static void Triangle_Init(uint32_t color)
{
    //Make the array bigger to hold the new pointer to a triangle:
    arrayTri = (Triangle*)realloc(arrayTri, (arrayTriLastPos + 1) * sizeof(Triangle*));
    //Add one triangle to the array:
    arrayTri[arrayTriLastPos] = (Triangle*) malloc(sizeof(Triangle));
    arrayTri[arrayTriLastPos]->color = color;
}

static void Triangle_AddPoint(int16_t nPoint, int x, int y)
{
    if(nPoint >= 3 )
    {
        printf("Error: Triangles only have three points!\n");
        return;
    }
    arrayTri[arrayTriLastPos]->v[nPoint].x = x;
    arrayTri[arrayTriLastPos]->v[nPoint].y = y;
    printf("New point %d for triangle at x %d y %d \n", nPoint, x, y);
    printf("arrayTri[arrayTriLastPos] == arrayTri[%d] ==  %d \n", arrayTriLastPos, arrayTri[arrayTriLastPos]);
}

static void Triangle_DestroyLast()
{
    printf("free(arrayTri[arrayTriLastPos]) == free(%d) \n", arrayTri[arrayTriLastPos]);
    free(arrayTri[arrayTriLastPos]);
    arrayTri = (Triangle*)realloc(arrayTri, arrayTriLastPos * sizeof(Triangle*));
}
////////////////////////////////////////////////////////////

//The counter of placed points for the new triangle or polygon:
static int16_t nPoint = 0;
void Editor_EventsHandler(SDL_Event* e)
{
    if(GUI_EventButtons(e))        //If a click was registered on any of the gui elements, return;
        return;
    //If we continue, it means the click was on a drawable part of the surface:

    //Get mouse position:
    int x, y;
    SDL_GetMouseState( &x, &y );

    switch(editorState)
    {
        case None:

            break;
        case Add_New_Point_To_Triangle:

            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                if(nPoint == 0)
                    Triangle_Init(rand());  //Passing random color.

                //Then we specify the point:
                Triangle_AddPoint(nPoint, x, y);

                //If it was the last one, the next click has to start again at point 0.
                if(++nPoint == 3)
                {
                    arrayTriLastPos++;
                    nPoint = 0;
                }


            }
            break;

        case Add_New_Point_To_Poly:

            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                if(nPoint == 0 || nPoint == 3)
                    //If it's the first vertex of a new triangle, we need to allocate memory:
                    Triangle_Init(rand());  //Pass random color.

                if(nPoint < 3)
                {
                    Triangle_AddPoint(nPoint, x, y);
                    if(++nPoint == 3)
                        arrayTriLastPos++;
                }
                else
                {
                    //If we are already working with a polygon more complex than a triangle,
                    //we specify the last two points that we already know:
                    Triangle* tempArray = arrayTri[arrayTriLastPos-1];
                    Triangle_AddPoint(0, tempArray->v[1].x, tempArray->v[1].y);
                    Triangle_AddPoint(1, tempArray->v[2].x, tempArray->v[2].y);

                    //We specify the new point:
                    Triangle_AddPoint(2, x, y);

                    arrayTriLastPos++;
                }


                //If we are already working with a polygon more complex than a triangle,
                //the next click has to create a new triangle:


            }
            break;  //When we are drawing a triangle and then change to a poly, or vice-versa, the triangle
                    //starts being drawn again, and the pointer to the triangle that was being drawn before
                    //GETS LOTS! MEMORY LEAK! Solved I think...
    }
}

void Editor_Draw()
{
    GUI_DrawButtons();
    //if(arrayTriSize > 0)
    for(int i = 0; i < arrayTriLastPos; i++)
        TriangleFlat(   arrayTri[i]->v[0], arrayTri[i]->v[1], arrayTri[i]->v[2],
                        arrayTri[i]->color, editorSurface);
}

void Editor_Change_State(States newState)
{
    //We close the current state:
    switch(editorState)
    {
        case None:
            break;
        case Add_New_Point_To_Triangle:

            //break;
        case Add_New_Point_To_Poly:
            if(nPoint > 0 && nPoint < 3)  //If we are in the middle of creating a new triangle.
            {
                printf("About to destroy the last traignle.\n");
                Triangle_DestroyLast();
                printf("Destroyed the last traignle.\n");
            }
            nPoint = 0;
            break;
        default:
            printf("Invalid current state!\n");
            break;
    }
    //We change to the next state and prepare it:
    editorState = newState;

    switch(editorState)
    {
        case None:
            break;
        case Add_New_Point_To_Triangle:
            break;
        case Add_New_Point_To_Poly:
            break;
        default:
            printf("Invalid new state!\n");
            break;
    }
    printf("Getting out of Editor_Change_state.\n");
}
