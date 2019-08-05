#include "editor_gui_objects.h"

//The states that the editor can be in:
typedef enum States {   None, Add_New_Point_To_Poly, Add_New_Surface,
                        Move_Poly_Point} States;

//typedef enum DrawableType { DNone = 0, DPoly, DSurface } DrawableType;

typedef struct Poly {
    uint16_t zOrder;
    Point** points;              //A pointer to a dynamic array of pointers to triangles (That will be in the array of triangles).
    uint16_t pointsSize;        //The size of the array of points.
    uint32_t color;             //The color of the polygon.
} Poly;

typedef struct ButtonPoint {    //A structure that contains a button associated to a point in a polygon, to be able to
    GUI_Button* button;         //select it and move it.
    Point* point;
} ButtonPoint;

//Later I could try implementing different types of Polys, maybe adding a
//typedef enum PolyType { Flat, Textured, Gouraud, Dither, DitherTransparency, Noise(rand()) and other effects } PolyType;
//and a PolyType type member inside of the sctruct. Then implementing the appropiate
//rendering functions in shapes.c

typedef struct Surface {
    uint16_t zOrder;
    SDL_Surface* surface;
    uint16_t x, y, w, h;        //We can stretch! I don't know if this is the best way to do it.
}   Surface;

//dynamicly growing arrays of surfaces and polygons, and buttons associated to points.:
static Poly*    arrayPoly = NULL;
static uint16_t arrayPolySize = 0;

static Surface* arraySurf = NULL;
static uint16_t arraySurfSize = 0;

static ButtonPoint* arrayButtPnt = NULL;
static uint16_t     arrayButtPntSize = 0;

//Some globals for this file:
static SDL_Surface* editorSurface = NULL;   //The surface that will be drawn by the editor.

static TTF_Font *editorFont = NULL;         //The font that will be used by the editor.

static States editorState;                  //The current state of the editor. Initialized in Editor_Init.

static uint32_t mouseState = 0;
static int mouseX = 0;
static int mouseY = 0;

//Why separate the concept of triangle and poly? A triangle IS a poly.
//Why write different cases for each? It's unnecessarily complicating everything.
//So let's fix that.
//There will be practically no efficiency saving in having triangles separated from more complex polys.

//Button's Functions: //////////////////////////////////////////////////////////////////////
static void clickNone()
{
    Editor_Change_State(None);
}

static void clickStartNewPoly()
{
    Editor_Change_State(Add_New_Point_To_Poly);
}
static void clickStartNewSurface()
{
    Editor_Change_State(Add_New_Surface);
}

static void clickPoint()
{
    Editor_Change_State(Move_Poly_Point); //Something like this.
    //It should be something like:  Press one of the buttons of the polys.
    //                              Change editor to state in which that point is moved.
    //                              Change to normal state when the click goes up.
}
static void unClickPoint()
{
    Editor_Change_State(None);
}
static void passedOverPoint()
{
    //If this function was called it means that the mouse moved on top
    //of a button of one of the polys.

}
////////////////////////////////////////////////////////////////////////////////////////////

void Editor_Init(SDL_Surface* surface)
{
    editorFont = TTF_OpenFont("synchronizer_nbp.ttf", 8); //Check for error.
    if(editorFont == NULL)
        printf("TTF_OpenFont failed! Probably the font is missiong! TTF_GetError: %s\n", TTF_GetError());

    editorSurface = surface;

    editorState = None;

    mouseState = 0;

    GUI_Init(editorSurface);

    GUI_CreateTextButton(   clickNone, NULL, NULL, "go to test1", editorFont,
                            editorSurface->w - 60, 10, 40, 30, 0xFF000000);
    GUI_CreateTextButton(   clickStartNewPoly, NULL, NULL, "go to test2", editorFont,
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
    //Get mouse state:
    mouseState = SDL_GetMouseState( &mouseX, &mouseY ); //Fill those globals every cicle.

    if(GUI_EventButtons(e))        //If a click was registered on any of the gui elements, return;
        return;
    //If we continue, it means the click was on a drawable part of the surface:
    switch(editorState)
    {
        case None:
//            if(e->type == SDL_MOUSEMOTION)
//            {
//                if((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)))
//                    ;
//            }

            break;
        case Add_New_Point_To_Poly:
        ///////////////////////////////////////////////////////////////////////////////////////
        {
            Poly* workPoly = &(arrayPoly[arrayPolySize - 1]);
            if(e->type == SDL_MOUSEBUTTONDOWN)
            {
                //When we add a new point, we need to allocate new memory for what will point to it in the array:
                workPoly->points = realloc(workPoly->points, ++(workPoly->pointsSize) * sizeof(Point*));

                //If we are NOT placing point in last pos, push elements in the array:
                if(whereToPutPoint < (workPoly->pointsSize - 1))
                {
                    for(int i = workPoly->pointsSize - 1; i > whereToPutPoint; i--)
                        workPoly->points[i]= workPoly->points[i - 1];

                    printf("whereToPutPoint special %d\n", whereToPutPoint);
                }
                //Add one point to the array:
                workPoly->points[whereToPutPoint] = malloc(sizeof(Point));
                //The values of the new point:
                workPoly->points[whereToPutPoint]->x = mouseX;
                workPoly->points[whereToPutPoint]->y = mouseY;

                //For each point added we need a button associated with it, to be able to manipulate it:

                //Create button for correspondent point:
                arrayButtPnt = realloc(arrayButtPnt, ++arrayButtPntSize * sizeof(ButtonPoint));

                arrayButtPnt[arrayButtPntSize-1].button =
                                GUI_CreateTextButton(clickPoint, passedOverPoint, unClickPoint, "b", editorFont, mouseX, mouseY, 12, 12, BLUE);
                arrayButtPnt[arrayButtPntSize-1].point = workPoly->points[whereToPutPoint];

                printf("Assigned button: %p to point: %p \n", arrayButtPnt[arrayButtPntSize-1].button, arrayButtPnt[arrayButtPntSize-1].point);

                whereToPutPoint++;
            }
            //////////
            else if(e->type == SDL_MOUSEMOTION)
            {
                //And assign the addecuate values to said point:
//                workPoly->points[workPoly->pointsSize - 1].x = mouseX;
//                workPoly->points[workPoly->pointsSize - 1].y = mouseY;
                //printf("x: %d, y: %d \n", mouseX, mouseY);
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
        case Move_Poly_Point:
        {
            GUI_Button* button = GUI_GetLastButtonDown();
            button->x = mouseX;
            button->y = mouseY;

            //Lets find out which point is associated to this button:
            for(int i = 0; i < arrayButtPntSize; i++)
            {
                if(arrayButtPnt[i].button == button)
                {   //And also change its coordinates:
                    printf("Moving assigned button %p to point %p\n", arrayButtPnt[i].button, arrayButtPnt[i].point);
                    //printf("%p\n", arrayButtPnt[i].point);
                    arrayButtPnt[i].point->x = mouseX;
                    arrayButtPnt[i].point->y = mouseY;
                    //printf("arrayButtPnt[i].point->x: %f, arrayButtPnt[i].point->y: %f \n", arrayButtPnt[i].point->x, arrayButtPnt[i].point->y );
                    break;
                }
            }


            break;
        }
        ///////////////////////////////////////////////////////////////////////////////////////
    }
}

void Editor_Update()
{

}

void Editor_Draw()
{
    //Draw polys:
    for(int i = 0; i < arrayPolySize; i++)
        PolyFlat(arrayPoly[i].points, arrayPoly[i].pointsSize, arrayPoly[i].color, editorSurface);

    //Draw surfaces:

    //Draw Gui:
    GUI_DrawButtons();

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
        case Move_Poly_Point:
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
            arrayPoly[arrayPolySize - 1].points = malloc(sizeof(Point*));
            arrayPoly[arrayPolySize - 1].color = 0xFF00FF0F;
            arrayPoly[arrayPolySize - 1].pointsSize = 0;
            arrayPoly[arrayPolySize - 1].zOrder = 0;
            whereToPutPoint = 0;
            break;
        case Move_Poly_Point:
            break;
        default:
            printf("Invalid new state!\n");
            break;
    }
    printf("Getting out of Editor_Change_state.\n");
}
