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
            pixels[currentXToDraw + surfaceW * scanlineY] = 0; //0, or black, only for now.

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

    float invSlope1 = (v3.x - v1.x) / (v3.y - v1.y); //the change of currentX1 per y-step.
    float invSlope2 = (v3.x - v2.x) / (v3.y - v2.y); //the change of currentX2 per y-step.

    float currentX1 = v1.x; //We start from the bottom.
    float currentX2 = v1.x; //We start from the bottom.

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
            pixels[currentXToDraw + surfaceW * scanlineY] = 0; //0, or black, only for now.

            //rand() instead of 0 for cool effect?

        //Calculate the next X1 and X2 to draw the next line:
        currentX1 -= invSlope1;
        currentX2 -= invSlope2;
    }
}
