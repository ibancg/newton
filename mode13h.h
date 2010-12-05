// Mode13h - Mode 13h graphics wrapper for SDL
// Copyright 2010, Ibán Cereijo Graña <ibancg at gmail dot com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __GRAPH_H
#define __GRAPH_H

// platform selector, when present, it assumes MS-DOS in protected mode,
// otherwise, SDL will be chosen.
//#define DOS

#ifndef DOS
#  include <SDL/SDL.h>
#endif

#include <stdio.h>

//--------------------------------------------------------------------------

// graphics handler struct
typedef struct {
#   ifdef DOS
	short int vid_selector; // video memory selector
	unsigned char* virtual;
#	else
	SDL_Surface* surface;
	SDL_Color colors[256];
#	endif
	unsigned char* pointer; // pointer to the virtual screen
	unsigned int size; // size in bytes
	unsigned int w; // width
	unsigned int h; // height
	double aspect_ratio_correction; // correction when the pixels aren't squares
} screen_t;

// initializes graphics session.
screen_t* startGraph(int double_buffer);

// finalizes graphics session.
void stopGraph(screen_t* screen);

// check whether a finalization event has been requested (key pressed or close
// button pressed).
unsigned int checkStopEvent(screen_t* screen);

// put and get pixel functions.
void putPixel(screen_t* screen, int x, int y, unsigned char color);
unsigned char getPixel(screen_t* screen, int x, int y);

// define a color, must be used with setPalette() function.
void defineColor(screen_t* screen, unsigned char, unsigned char, unsigned char,
		unsigned char);
void setPalette(screen_t* screen);

// copies the virtual screen into the physical one.
void flip(screen_t* screen);

#endif
