#include "shapes.h"

// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#sunbresenhamarticle

FillUpTriangle(Vertice v1, Vertice v2, Vertice v3, SDL_Surface* surface)
{
    //        v1 *
    //          / \
    //         /   \
    //        /     \
    //    v2 *_______* v3   from top to bottom! v1.y < v2.y
    //                                          v2.y == v3.y
    //                                          v2.x < v3.x



    float invSlope1 = (v2.x - v1.x) / (v2.y - v1.y); //the change of currentX1 per y-step.
    float invSlope2 = (v3.x - v1.x) / (v3.y - v1.y); //the change of currentX2 per y-step.

    float currentX1 = v1.x; //We start from the top.
    float currentX2 = v1.x; //We start from the top.

    //pointer to the pixels of the surface:
    uint32_t *pixels = surface->pixels;
    //Let's access the width only once and save them for later:
    uint16_t surfaceW = surface->w;

    //for every scanline:
    for(int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
    {
        //draw current scanline, between currentX1 and currentX2:
        for(int currentXToDraw = currentX1; currentXToDraw <= currentX2; currentXToDraw++)
            //Draw a point:
            pixels[currentXToDraw + surfaceW * scanlineY] = 0xFFFF0000; //Hardcoded, for now.

            //rand() instead of 0 for cool effect?

        //Calculate the next X1 and X2 to draw the next line:
        currentX1 += invSlope1;
        currentX2 += invSlope2;
    }
}

FillDownTriangle(Vertice v1, Vertice v2, Vertice v3, SDL_Surface* surface)
{
    //    v1 *-------* v2
    //        \     /
    //         \   /
    //          \ /
    //           * v3       from bottom to top! v3.y > v1.y
    //                                          v1.y == v2.y
    //                                          v2.x > v1.x

    float invSlope1 = (v3.x - v1.x) / (v3.y - v1.y); //the change of currentX1 per y-step.
    float invSlope2 = (v3.x - v2.x) / (v3.y - v2.y); //the change of currentX2 per y-step.

    float currentX1 = v3.x; //We start from the bottom.
    float currentX2 = v3.x; //We start from the bottom.

    //pointer to the pixels of the surface:
    uint32_t *pixels = surface->pixels;
    //Let's access the width only once and save them for later:
    uint16_t surfaceW = surface->w;

    //for every scanline:
    for(int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
    {
        //draw current scanline, between currentX1 and currentX2:
        for(int currentXToDraw = currentX1; currentXToDraw <= currentX2; currentXToDraw++)
            //Draw a point:
            pixels[currentXToDraw + surfaceW * scanlineY] = 0xFF0000FF; //Hardcoded, for now.

            //rand() instead of 0 for cool effect?

        //Calculate the next X1 and X2 to draw the next line:
        currentX1 -= invSlope1;
        currentX2 -= invSlope2;
    }
}

FillTriangle(Vertice v1, Vertice v2, Vertice v3, SDL_Surface* surface)
{
    //        v1 *
    //          / \         from top to bottom! v1.y <= v2.y <= v3.y
    //         /   \                            v2.y == v4.y
    //        /     \                           v2.x < v4.x < v3.x
    //    v2 *-------* v4
    //         ---    \
    //            ---  \
    //               ---* v3


    //First, sort the vertices by y-coordinate ascending so V1 is the topmost vertice.
    Vertice helper;
    if(v1.y > v2.y)         //If v1.y is under v2.y
    {
        helper = v1;     //We swap them.
        v1 = v2;
        v2 = helper;
    }
    if(v1.y > v3.y)         //If v1.y is under v3.y
    {
        helper = v1;     //We swap them.
        v1 = v3;
        v3 = helper;
    }
    if(v2.y > v3.y)         //If v2.y is under v3.y
    {
        helper = v2;     //We swap them.
        v2 = v3;
        v3 = helper;
    }
    //Now I know that v1.y is on top, v2.y is in the middle and v3.y is the lowest,
    //in the case of a general triangle (No flat top or bottom).
    // v1.y <= v2.y <= v3.y

    //Ok, we shoud be ready now... I suppose:

    //General case - split the triangle in two; a top-flat and bottom-flat one:
    Vertice v4;
    v4.y = v2.y;
    v4.x = v1.x + ( (v2.y - v1.y) / (v3.y - v1.y) * (v3.x - v1.x) );
    //And draw them!:

    //Now, I also know that, for the flat Bottom (Up) case, we need v2.x < v3.x:
    if(v2.x > v3.x)         //if v2.x is more to the right than v3.x
    {
        helper = v2;      //We swap them.
        v2 = v3;
        v3 = helper;
    }

    FillUpTriangle(v1, v2, v3, surface);

    //And that for the flat top (Down) case, we need v2.x > v1.x, but in this case, v4.x > v2.x:
    //    v1 *-------* v2           v2 *-------* v4
    //        \     /                    ---    \
    //         \   /        =>              ---  \
    //          \ /                            ---* v3
    //           * v3

    if(v4.x < v2.x)
    {
        helper = v4;      //We swap them.
        v4 = v2;
        v2 = helper;
    }
    FillDownTriangle(v2, v4, v3, surface);

}//This isn't working as it should at is not as efficient either.

//http://www.hugi.scene.org/online/coding/hugi%2017%20-%20cotriang.htm
TriangleFlat(Vertex v1, Vertex v2, Vertex v3, SDL_Surface* surface)
{
    //        v1 *
    //          / \         from top to bottom! v1.y <= v2.y <= v3.y
    //         /   \                            v2.y == v4.y
    //        /     \                           v2.x < v4.x < v3.x?????
    //    v2 *-------* v4
    //         ---    \
    //            ---  \
    //               ---* v3

    //Sort the vertices by their y values:
    Vertex helper;
    if(v1.y > v2.y)         //If v1.y is under v2.y
    {
        helper = v1;     //We swap them.
        v1 = v2;
        v2 = helper;
    }
    if(v1.y > v3.y)         //If v1.y is under v3.y
    {
        helper = v1;     //We swap them.
        v1 = v3;
        v3 = helper;
    }
    if(v2.y > v3.y)         //If v2.y is under v3.y
    {
        helper = v2;     //We swap them.
        v2 = v3;
        v3 = helper;
    }

    //It's time to check if we have more than a zero height triangle.
    if(v1.y == v3.y)
        return;

    //Now we have to calculate 3 slopes. dXdY - means deltaX/deltaY.
    //That's gonna tell us how much to change x for every y increment.
    //dXdY_V1V2 - means dXdY for the edge V1 to V2.
    float dXdY_V1V3 = (v3.x - v1.x) / (v3.y - v1.y);
    float dXdY_V2V3 = (v3.x - v2.x) / (v3.y - v2.y);
    float dXdY_V1V2 = (v2.x - v1.x) / (v2.y - v1.y);

    //We have to decide whether V2 is on the left side or the right one
    //We could do that by finding the V4, and check the distance from
    // V4 to V2, distance = V4.x - V2.x :
    Vertex v4;
    v4.y = v2.y;
    //The next operation is derived from: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#sunbresenhamarticle
    v4.x = v1.x + ( (v2.y - v1.y) / (v3.y - v1.y) * (v3.x - v1.x) );

    float distance = v4.x - v2.x;

    float left_dXdY, right_dXdY, leftX, rightX;

    if(distance > 0)    //if (distance>0) the middle vertex v2 is on the left side
    {                   //(V1V3 is the longest edge on the right side)

        DrawSegment(v1.y, v2.y, v1.x, v1.x, dXdY_V1V2, dXdY_V1V3, surface);
        DrawSegment(v2.y, v3.y, v2.x, v4.x, dXdY_V2V3, dXdY_V1V3, surface);
    }
    else                //and if (distance<0) then V1V3 is on the left side.
    {
        left_dXdY   = dXdY_V1V3;
        right_dXdY  = dXdY_V1V2;
        DrawSegment(v1.y, v2.y, v1.x, v1.x, dXdY_V1V3, dXdY_V1V2, surface);
        DrawSegment(v2.y, v3.y, v4.x, v2.x, dXdY_V1V3, dXdY_V2V3, surface);
    }

    //A triangle is made out of two segments: v1.y to v2.y and v2.y to v3.y:
    //Let's draw the first segment: /////////////////////////////////////////////////////

    /*
    leftX = v1.x; //We start from the top.
    rightX = v1.x; //We start from the top.

    //pointer to the pixels of the surface:
    uint32_t *pixels = surface->pixels;
    //Let's access the width only once and save them for later:
    uint16_t surfaceW = surface->w;

    //for every scanline:
    for(int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
    {
        //draw current scanline, between currentX1 and currentX2:
        for(int currentXToDraw = leftX; currentXToDraw <= rightX; currentXToDraw++)
            //Draw a point:
            pixels[currentXToDraw + surfaceW * scanlineY] = 0xFFFF0000; //Hardcoded, for now.

        //Calculate the next X1 and X2 to draw the next line:
        leftX   += dXdY_V1V2;
        rightX  += dXdY_V1V3;
    }*/
    /////////////////////////////////////////////////////////////////////////////////////




    printf("v1.y: %f, v2.y: %f, v3.y: %f\n", v1.y, v2.y, v3.y);
}

//I could probably make those variables gobals, except for topY and bottomY.
static void DrawSegment( int topY, int bottomY, float leftX, float rightX,
                    float left_dXdY, float right_dXdY, SDL_Surface* surface)
{
    //pointer to the pixels of the surface:
    uint32_t *pixels = surface->pixels;
    //Let's access the width only once and save them for later:
    uint16_t surfaceW = surface->w;

    //for every scanline:
    for(int scanlineY = topY; scanlineY <= bottomY; scanlineY++)
    {
        //draw current scanline, between currentX1 and currentX2:
        for(int currentXToDraw = leftX; currentXToDraw <= rightX; currentXToDraw++)
            //Draw a point:
            pixels[currentXToDraw + surfaceW * scanlineY] = 0xFFFF0000; //Hardcoded, for now.

        //Calculate the next X1 and X2 to draw the next line:
        leftX   += left_dXdY;
        rightX  += right_dXdY;
    }
}









