// Mode13h - Mode 13h graphics wrapper for SDL
// Copyright 1995-2010, Ibán Cereijo Graña <ibancg at gmail dot com>
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

#include "mode13h.h"

#ifdef DOS
#  include <conio.h>
#  include <dpmi.h>
#endif

screen_t* startGraph(int double_buffer) {
	screen_t* result = malloc(sizeof(screen_t));
	result->w = 320;
	result->h = 200;
	result->size = result->w * result->h;
#   ifdef DOS
	_go32_dpmi_registers regs;
	regs.x.ax = 0x13;
	__dpmi_simulate_real_mode_interrupt(0x10, &regs);
	result->vid_selector = __dpmi_segment_to_descriptor(0xA000);
	result->virtual = NULL;
	if (double_buffer) {
		result->virtual = (unsigned char *) malloc(result->size);
		_go32_dpmi_lock_data(result->virtual, result->size);
		memset(result->virtual, 0, result->size);
	}

	// the pixels are not squares
	result->pointer = result->virtual;
	result->aspect_ratio_correction = 5.0 / 6.0;
#   else
	result->surface = SDL_SetVideoMode(result->w, result->h, 8, SDL_HWSURFACE
			| SDL_DOUBLEBUF);
	// the pixels are squares when they are rendered in a X window
	result->aspect_ratio_correction = 1.0;
	if (result->surface == NULL) {
		free(result);
		result = NULL;
	} else {
		result->pointer = result->surface->pixels;
	}
	//	memset(result->pointer, 100, 64000);
#   endif
	return result;
}

void stopGraph(screen_t* screen) {
#   ifdef DOS
	if (screen->virtual != NULL) {
		free(screen->virtual);
	}
	_go32_dpmi_registers regs;
	regs.x.ax = 0x3;
	__dpmi_simulate_real_mode_interrupt(0x10, &regs);
#   else
	SDL_Quit();
#   endif
	free(screen);
}

unsigned int checkStopEvent(screen_t* screen) {
	unsigned int stop = 0;
#   ifdef DOS
	stop = kbhit();
#   else
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) /*|| (event.type == SDL_KEYUP)*/) {
			stop = 1;
		}
	}
#   endif
	return stop;
}

inline void putPixel(screen_t* screen, int x, int y, unsigned char color) {
	if ((x >= 0) && (x < screen->w) && (y >= 0) && (y < screen->h)) {
#   	ifdef DOS
		if (screen->virtual != NULL) {
			*(((unsigned char*) screen->pointer) + (y << 6) + (y << 8) + x)
					= color;
		} else {
			_farpokeb(screen->vid_selector, (y << 6) + (y << 8) + x, color);
		}
#   	else
		char *buffer = (char*) screen->surface->pixels + screen->surface->pitch
		* y + x * screen->surface->format->BytesPerPixel;

		//	if (SDL_MUSTLOCK(screen))
		//		SDL_LockSurface(screen);

		*buffer = color;

		//	if (SDL_MUSTLOCK(screen))
		//		SDL_UnlockSurface(screen);
#   	endif
	}
}

unsigned char getPixel(screen_t* screen, int x, int y) {
	if ((x >= 0) && (x < screen->w) && (y >= 0) && (y < screen->h)) {
#   	ifdef DOS
		if (screen->virtual != NULL) {
			return *(((unsigned char*) screen->pointer) + (y << 6) + (y << 8)
					+ x);
		} else {
			return _farpeekb(screen->vid_selector, (y << 6) + (y << 8) + x);
		}
#   	else
		char *buffer = (char*) screen->surface->pixels + screen->surface->pitch
		* y + x * screen->surface->format->BytesPerPixel;

		//	if (SDL_MUSTLOCK(screen))
		//		SDL_LockSurface(screen);

		return *buffer;

		//	if (SDL_MUSTLOCK(screen))
		//		SDL_UnlockSurface(screen);
#   	endif
	}
}

void defineColor(screen_t* screen, unsigned char color_index, unsigned char r,
		unsigned char g, unsigned char b) {
#   ifdef DOS
	outportb(0x3C8, color_index);
	outportb(0x3C9, r >> 2);
	outportb(0x3C9, g >> 2);
	outportb(0x3C9, b >> 2);
#   else
	screen->colors[color_index].r = r;
	screen->colors[color_index].g = g;
	screen->colors[color_index].b = b;
#   endif
}

void setPalette(screen_t* screen) {
#   ifdef DOS
#   else
	SDL_SetColors(screen->surface, screen->colors, 0, 256);
#   endif
}

void flip(screen_t* screen) {
#   ifdef DOS
	if (screen->virtual != NULL) {
		//_movedatal(_my_ds(), screen->virtual, screen->vid_selector, 0, 16000);
		int i;
		unsigned long int* p = screen->virtual;
		for (i = 0; i < screen->size; i += 4)
			_farpokel(screen->vid_selector, i, *p++);
	}
#   else
	SDL_Flip(screen->surface);
#   endif
}
