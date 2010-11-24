#ifndef __GRAPH_H
#define __GRAPH_H

//#define DOS

#ifndef DOS
#  include <SDL/SDL.h>
#endif

#include <stdio.h>

//--------------------------------------------------------------------------

// body struct
typedef struct {
#   ifdef DOS
	short int vid_selector; // video memory selector
	unsigned char* virtual;
#	else
	SDL_Surface* surface;
	SDL_Color colors[256];
#	endif
	unsigned char* pointer;
	unsigned int size; // size in bytes
	unsigned int w;
	unsigned int h;
	double aspect_ratio_correction;
} screen_t;

screen_t* startGraph(int double_buffer);
void stopGraph(screen_t* screen);
unsigned int checkStopEvent(screen_t* screen);
void putPixel(screen_t* screen, int x, int y, unsigned char color);
void defineColor(screen_t* screen, unsigned char, unsigned char, unsigned char,
		unsigned char);
void setPalette(screen_t* screen);
void flip(screen_t* screen);

#endif
