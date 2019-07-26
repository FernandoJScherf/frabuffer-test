#ifndef EDITOR_GUI_OBJECTS_H_INCLUDED
#define EDITOR_GUI_OBJECTS_H_INCLUDED
//Let's build a simple gui with what I implemented in gui.h and gui.c:
//I will implement everything here just so I can keep main clean and tidy.

#include "gui.h"

void Editor_Init(SDL_Surface* surface);

void Editor_Quit();

void Editor_EventsHandler(SDL_Event* e);

void Editor_Draw();

void Editor_Update();

void Editor_Change_State();



#endif // EDITOR_GUI_OBJECTS_H_INCLUDED
