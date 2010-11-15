
/* Simulaci¢n de cuerpos en movimiento en el espacio, siguiendo el principio
 de gravitaci¢n de Newton. */

#include <stdio.h>
#include <math.h>
#include <conio.h>

#include "modo13.h"

#define dt 1.0e-2   // Resoluci¢n temporal de c lculo.
#define NC 100      // N£mero de c lculos entre representaciones.
#define MC 10       // M ximo n£mero de cuerpos.

const double E = 160.0 / 300.0; // Escala.
const double _083E = 0.83 * E;

#define sqr_M(x)   ((x)*(x))

class cuerpo {
 public:
  double px,py; // posici¢n.
  double vx,vy; // velocidad.
  double ax,ay; // aceleraci¢n.
  double k;     // constante proporcional a la masa (G*M).
  unsigned char color;
};

unsigned char *Virtual; // Pantalla virtual.
int           N = 0;
cuerpo        C[MC];


void CreaCuerpo(double, double, double, double, double, unsigned char = 255);

void main() {

double        ax,ay,p1;
int	      i,j,n;
unsigned char *v;


  Inicia(0x13);
  Virtual = (unsigned char *) malloc(64000);
  _go32_dpmi_lock_data(Virtual, 64000);
  memset(Virtual,0,64000);


/*  CreaCuerpo( 0.0  ,0.0,  0.0,0.0,  400.0);
  CreaCuerpo( 149.0,0.0,  -1.0,-1.0,  1.0);
  CreaCuerpo( 34.0,0.0,  -1.0,1.0,  1.0);
  CreaCuerpo( 64.0,0.0,  -1.0,1.0,  1.0);
  CreaCuerpo( 100.0,0.0,  -1.0,1.0,  1.0);
  CreaCuerpo( 200.0,0.0,  -1.0,1.0,  140.0);*/

  CreaCuerpo( 0.0  ,0.0,  0.0,0.0,  200.0);
  CreaCuerpo( 50.0,40.0,  -1.0,1.0,  100.0);
  CreaCuerpo( -150.0,40.0,  1.0,-1.2,  100.0);

//  CreaCuerpo( 0.0  ,0.0,  0.0,0.0,  100.0);
  CreaCuerpo( 100.0  ,0.0,  -0.3,1.0,  0.0);

  for (i = 0; i < 256; i++) PonPaleta(i, i >> 2, i >> 2, i >> 3);
  PonPaleta(255, 60,0,0);

  for (n = 0; !kbhit(); n++) {
    for (i = 0; i < N; i++) {
       C[i].ax = C[i].ay = 0.0;
       for (j = 0; j < N; j++) if (i != j) {
	 p1 = C[j].k / pow(sqr_M(C[i].px - C[j].px) + sqr_M(C[i].py - C[j].py),1.5);
	 C[i].ax += (C[j].px - C[i].px) * p1;
	 C[i].ay += (C[j].py - C[i].py) * p1;
       }

    }

    for (i = 0; i < N; i++) {
       C[i].vx += C[i].ax * dt;
       C[i].vy += C[i].ay * dt;
       C[i].px += C[i].vx * dt;
       C[i].py += C[i].vy * dt;
    }

    if (n == NC) {

      for (j = 0, v = Virtual; j < 64000; j++, v++) if (*v > 40) *v = *v - 1;
//      memset(Virtual,0,64000);
      for (i = 0; i < N; i++)
  	PonPixel13(160 + C[i].px * E,100 - C[i].py * _083E,C[i].color,Virtual);

      _movedatal(_my_ds(), Virtual, vid_selector, 0, 16000);
      n = 0;
    }
  }

  getchar();
  Inicia(3);
}

/* Crea un cuerpo pasando como par metros la posici¢n y velocidad iniciales,
 la masa y el color. */
void CreaCuerpo(double pox, double poy, double vox, double voy, double M, unsigned char color)
{
   C[N].px = pox;
   C[N].py = poy;
   C[N].vx = vox;
   C[N].vy = voy;
   C[N].k = M;
   C[N].color = color;
   N++;
}

