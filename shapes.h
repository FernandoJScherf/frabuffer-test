#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#sunbresenhamarticle
// http://www.hugi.scene.org/online/coding/hugi%2017%20-%20cotriang.htm

// Later I could implement some "other types" of triangles: textured, gourald?, noise, etc.

typedef struct Vertice {
    float x, y;
} Vertice;

void FillUpTriangle(Vertice v1, Vertice v2, Vertice v3, SDL_Surface* surface);
void FillDownTriangle(Vertice v1, Vertice v2, Vertice v3, SDL_Surface* surface);
void FillTriangle(Vertice v1, Vertice v2, Vertice v3, SDL_Surface* surface);

//Let's try to do it better:
//http://www.hugi.scene.org/online/coding/hugi%2017%20-%20cotriang.htm
typedef struct Vertex {
    float x, y;
} Vertex;

void TriangleFlat(Vertex v1, Vertex v2, Vertex v3, SDL_Surface* surface);
static void DrawSegment( int topY, int bottomY, float leftX, float rightX,
                    float left_dXdY, float right_dXdY, SDL_Surface* surface);

#endif // SHAPES_H_INCLUDED
