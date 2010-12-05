// Newton - Modeling and simulation of planetary motion
// Copyright 1999-2010, Ibán Cereijo Graña <ibancg at gmail dot com>
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mode13h.h"

#define calculations_per_plot 5 // Calculations between two representations
#define max_nbodies 10 // Maximum body number
#define G 6.6743e-11 // gravitational constant
#define sqr_M(x)   ((x)*(x))

// body struct
typedef struct {
	double px, py; // position
	double vx, vy; // velocity
	double ax, ay; // acceleration
	double k; // mass constant (G*M)
	unsigned char fixed; // indicates whether or not the body is moving
} body_t;

// create a body given the initial position (m), velocity (m/s), and mass (Kg)
int createBody(body_t* body, double pox, double poy, double vox, double voy,
		double mass, unsigned char fixed) {
	body->px = pox;
	body->py = poy;
	body->fixed = fixed;
	body->vx = vox;
	body->vy = voy;
	body->k = G * mass;
	return 1;
}

int main(int argc, char* argv[]) {

	int nbodies = 0;
	body_t body[max_nbodies];

	double p1;
	double t = 0.0; // time
	int i, j, k, n = 0; // iteration variables
	body_t *bi, *bj;
	double x, y;
	unsigned char done = 0;
	unsigned char* v;

	screen_t* screen = startGraph(1);

	if (screen == NULL) {
		perror("cannot set vido mode");
		exit(-1);
	}

	//	// earth - moon system
	//	const double scale_factor = screen->h / 800.0e6; // 800000 Km height
	//	const double delta_time = 10.0; // Temporal calculus resolution
	//	nbodies += createBody(&body[nbodies], 0.0, 0.0, 0.0, 0.0, 5.9736e24, 1);
	//	nbodies
	//			+= createBody(&body[nbodies], 384400e3, 0.0, 0.0, 1022, 7.349e22, 0);

	const double scale_factor = screen->h / 300.0; // 300 m height
	const double delta_time = 2e-2; // Temporal calculus resolution
	nbodies += createBody(&body[nbodies], 50.0, 40.0, -0.2, 0.2, 100.0 / G, 0);
	nbodies += createBody(&body[nbodies], -150.0, -40.0, 0.2, -0.3, 100.0 / G,
			0);
	//			nbodies += createBody(&body[nbodies], 50.0, -70.0, 0.2, 0.4, 300.0 / G, 0);

	sleep(2);

	body_t* bodyref = &body[0];
	//	body_t* bodyref = NULL;

	const double scale_factor_x = scale_factor;
	const double scale_factor_y = scale_factor
			* screen->aspect_ratio_correction;
	const unsigned int xc = screen->w / 2;
	const unsigned int yc = screen->h / 2;

	// color palette definition: yellow gradient
	for (i = 0; i < 255; i++) {
		defineColor(screen, i, i, i, i >> 2);
	}
	defineColor(screen, 255, 255, 0, 0);
	setPalette(screen);

	n = 0;
	do {

		// compute all the forces between pairs, and hence the accelerations.
		for (i = 0; i < nbodies; i++) {
			bi = &body[i];
			// I'll accumulate the accelerations, since the forces can be added
			bi->ax = bi->ay = 0.0;
			for (j = 0; j < nbodies; j++)
				if (i != j) {
					bj = &body[j];
					p1 = bj->k / pow(sqr_M(bi->px - bj->px)
							+ sqr_M(bi->py - bj->py), 1.5);
					bi->ax += (bj->px - bi->px) * p1;
					bi->ay += (bj->py - bi->py) * p1;
				}
		}

		// time integration
		for (i = 0; i < nbodies; i++) {
			bi = &body[i];
			if (!bi->fixed) {
				bi->vx += bi->ax * delta_time;
				bi->vy += bi->ay * delta_time;
				bi->px += bi->vx * delta_time;
				bi->py += bi->vy * delta_time;
			}
		}

		if (n++ == calculations_per_plot) {

			// we darken the body path

			for (j = 0, v = screen->pointer; j < screen->size; j++, v++) {
				if (*v > 40) {
					(*v)--;
				}
			}

			for (i = 0; i < nbodies; i++) {
				x = body[i].px;
				y = body[i].py;

				if (bodyref != NULL) { // relative motion
					x -= bodyref->px;
					y -= bodyref->py;
				}
				putPixel(screen, xc + x * scale_factor_x, yc - y
						* scale_factor_y, 255);
			}

			flip(screen);
			done = checkStopEvent(screen);

			n = 0;
		}

		t += delta_time;
		//				printf("t = %f s\n", t);

	} while (!done);

	stopGraph(screen);

	return 0;
}

