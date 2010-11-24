#include "mode13h.h"

#ifdef DOS
#  include <conio.h>
#  include <dpmi.h>
#endif

screen_t* startGraph(int double_buffer) {
	screen_t* result = malloc(sizeof(screen_t));
	result->w = 320;
	result->h = 200;
#   ifdef DOS
	_go32_dpmi_registers regs;
	regs.x.ax = 0x13;
	__dpmi_simulate_real_mode_interrupt(0x10, &regs);
	result->vid_selector = __dpmi_segment_to_descriptor(0xA000);
	result->virtual = 0xA0000000;
	if (double_buffer) {
		result->virtual = (unsigned char *) malloc(64000);
	}
	_go32_dpmi_lock_data(result->virtual, 64000);
	memset(result->virtual, 0, 64000);
	// the pixels are not squares
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
		result->size = result->surface->w * result->surface->h
				* result->surface->format->BytesPerPixel;
	}
	//	memset(result->pointer, 100, 64000);
#   endif
	return result;
}

void stopGraph(screen_t* screen) {
#   ifdef DOS
	if (screen->virtual != 0xA0000000) {
		free(screen->virtual);
	}
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
		if ((event.type == SDL_QUIT) || (event.type == SDL_KEYUP)) {
			stop = 1;
		}
	}
	// TODO: check key pressed
#   endif
	return stop;
}

inline void putPixel(screen_t* screen, int x, int y, unsigned char color) {
	if ((x >= 0) && (x < screen->w) && (y >= 0) && (y < screen->h)) {
#   	ifdef DOS
		*(((unsigned char*) screen->pointer) + (y << 6) + (y << 8) + x) = c;
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

void defineColor(screen_t* screen, unsigned char color_index, unsigned char r,
		unsigned char g, unsigned char b) {
#   ifdef DOS
	outportb(0x3C8, color_index);
	outportb(0x3C9, r);
	outportb(0x3C9, g);
	outportb(0x3C9, b);
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
	_movedatal(_my_ds(), screen->virtual, screen->vid_selector, 0, 16000);
	//	int i;
	//	for (i = 0; i < 64000; i += 4)
	//		_farpokel(vid_selector, i, *p++);
#   else
	SDL_Flip(screen->surface);
#   endif
}
