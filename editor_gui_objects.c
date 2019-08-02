#include "editor_gui_objects.h"

//The states that the editor can be in:
typedef enum States {   None, Add_New_Point_To_Poly, Add_New_Surface  } States;

//typedef enum DrawableType { DNone = 0, DPoly, DSurface } DrawableType;

typedef struct Poly {
    uint16_t zOrder;
    Point* points;              //A pointer to a dynamic array of pointers to triangles (That will be in the array of triangles).
    uint16_t pointsSize;        //The size of the array of points.
    uint32_t color;             //The color of the polygon.
} Poly;

//Later I could try implementing different types of Polys, maybe adding a
//typedef enum PolyType { Flat, Textured, Gouraud, Dither, DitherTransparency, Noise(rand()) and other effects } PolyType;
//and a PolyType type member inside of the sctruct. Then implementing the appropiate
//rendering functions in shapes.c

typedef struct Surface {
    uint16_t zOrder;
    SDL_Surface* surface;
    uint16_t x, y, w, h;        //We can stretch! I don't know if this is the best way to do it.
}   Surface;

//dynamicly growing arrays of surfaces and polygons:
static Poly* arrayPoly = NULL;
static uint16_t arrayPolySize = 0;

static Surface* arraySurf = NULL;
static uint16_t arraySurfSize = 0;

//Some globals for this file:
static SDL_Surface* editorSurface = NULL;   //The surface that will be drawn by the editor.

static TTF_Font *editorFont = NULL;         //The font that will be used by the editor.

static States editorState;                  //The current state of the editor. Initialized in Editor_Init.

//Why separate the concept of triangle and poly? A triangle IS a poly.
//Why write different cases for each? It's unnecessarily complicating everything.
//So let's fix that.
//There will be practically no efficiency saving in having triangles separated from more complex polys.

//Button's Functions: //////////////////////////////////////////////////////////////////////
static void test1()
{
    printf("click1. \n");
    Editor_Change_State(None);
}
static void test2()
{
    printf("click2. \n");
    Editor_Change_State(Add_New_Point_To_Poly);
}
static void test3()
{
    printf("click3. \n");
    Editor_Change_State(Add_New_Surface);
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
                            editorSurface->w - 60, 10, 40, 30, 0xFF000000);
    GUI_CreateTextButton(   test2, "go to test2", editorFont,
                            editorSurface->w - 60, 60, 40, 30, 0xFF000000);
}

void Editor_Quit()
{
    //Put everything that should be NULLed here.
    SDL_FreeSurface(editorSurface);
    TTF_CloseFont(editorFont);
    GUI_Quit();
}

//The counter of placed points for the new triangle or polygon:
static uint16_t whereToPutPoint = 0;
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
        case Add_New_Point_To_Poly:
        ///////////////////////////////////////////////////////////////////////////////////////
        {
            Poly* workPoly = &(arrayPoly[arrayPolySize - 1]);
            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                //When we add a new point, we need to allocate new memory for a new point:

                workPoly->points = realloc(workPoly->points, ++(workPoly->pointsSize) * sizeof(Point));

                printf("%d, %d\n", arrayPoly[arrayPolySize - 1].points, arrayPoly[arrayPolySize - 1].pointsSize);

                //And assign the addecuate values to the points:
                if(whereToPutPoint >= (workPoly->pointsSize - 1))
                {
                    workPoly->points[whereToPutPoint].x = x;
                    workPoly->points[whereToPutPoint].y = y;
                    printf("whereToPutPoint normal %d\n", whereToPutPoint);
                }
                else
                {
                    for(int i = workPoly->pointsSize - 1; i > whereToPutPoint; i--)
                        workPoly->points[i] = workPoly->points[i - 1];

                    workPoly->points[whereToPutPoint].x = x;
                    workPoly->points[whereToPutPoint].y = y;
                    printf("whereToPutPoint special %d\n", whereToPutPoint);
                }
                whereToPutPoint++;
            }
            //////////
            else if(e->type == SDL_MOUSEMOTION && workPoly->pointsSize > 2)
            {
                //And assign the addecuate values to said point:
//                workPoly->points[workPoly->pointsSize - 1].x = x;
//                workPoly->points[workPoly->pointsSize - 1].y = y;
                //printf("x: %d, y: %d \n", x, y);
            }
            //////////
            else if(e->type == SDL_KEYDOWN)
            {
                switch( e->key.keysym.sym )
                {
                    case SDLK_RIGHT:
                        if(whereToPutPoint >= (workPoly->pointsSize - 1))
                            whereToPutPoint = 1;
                        else
                            whereToPutPoint++;

                        break;
                    case SDLK_LEFT:

                        break;
                }
            }
        }
            break;
        ///////////////////////////////////////////////////////////////////////////////////////
    }
}

void Editor_Update()
{

}

void Editor_Draw()
{
    GUI_DrawButtons();

    //Draw polys:
    for(int i = 0; i < arrayPolySize; i++)
    {
        PolyFlat(arrayPoly[i].points, arrayPoly[i].pointsSize, arrayPoly[i].color, editorSurface);

        for(int j = 0; j < arrayPoly[i].pointsSize; j++)
        {
            //Mini triangle to mark a point:
            Point v1, v2, v3;
            v1.x = arrayPoly[i].points[j].x - 4;   //HARDCODING IS BAD!
            v1.y = arrayPoly[i].points[j].y + 4;
            v2.x = arrayPoly[i].points[j].x;        //HARDCODING IS BAD!
            v2.y = arrayPoly[i].points[j].y - 4;
            v3.x = arrayPoly[i].points[j].x + 4;   //HARDCODING IS BAD! Also, this could be buttons!!!
            v3.y = arrayPoly[i].points[j].y + 4;
            TriangleFlat(v1, v2, v3, j * 50, editorSurface);
        }

    }

    //Draw surfaces:

}

void Editor_Change_State(States newState)
{
    //We close the current state:
    switch(editorState)
    {
        case None:
            break;
        case Add_New_Point_To_Poly:
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
        case Add_New_Point_To_Poly:

            //If it's the first point of a new poly, we need to allocate memory:
            arrayPoly = realloc(arrayPoly, ( ++arrayPolySize ) * sizeof(Poly));
            arrayPoly[arrayPolySize - 1].points = malloc(sizeof(Point));
            arrayPoly[arrayPolySize - 1].color = 0xFF00FF0F;
            arrayPoly[arrayPolySize - 1].pointsSize = 0;
            arrayPoly[arrayPolySize - 1].zOrder = 0;
            whereToPutPoint = 0;
            break;
        default:
            printf("Invalid new state!\n");
            break;
    }
    printf("Getting out of Editor_Change_state.\n");
}
