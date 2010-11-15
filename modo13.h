#ifndef __MODO13_H
#define __MODO13_H

#include <dpmi.h>
#include <stdio.h>

#define swp(a,b,c)  c = a; a = b; b = c

unsigned char *VGA = (unsigned char *) 0xA0000000;

/*--------------------------------------------------------------------------*/
void Inicia(int);
inline void PonPixel13(int, int, unsigned char);
inline unsigned char CogePixel13(int, int);
inline void PonPixel13(int, int, unsigned char, unsigned char *);
inline unsigned char CogePixel13(int, int, unsigned char *);
void Linea(int, int, int, int, unsigned char);
void Linea(int, int, int, int, unsigned char, unsigned char *);
void Circulo(int, int, int, unsigned char);
void Circulo(int, int, int, unsigned char, unsigned char *);
void PonPaleta(unsigned char, unsigned char, unsigned char, unsigned char);
void CogePaleta(unsigned char, unsigned char &, unsigned char &, unsigned char &);
void Flip(long int *);
void Retrazo();
void PonPcx(char *,unsigned char * = NULL);

/*--------------------------------------------------------------------------*/

short int            vid_selector; //Selector para la memoria de v¡deo.
_go32_dpmi_registers regs;

void Inicia(int modo)
{

  regs.x.ax = modo;
  __dpmi_simulate_real_mode_interrupt(0x10, &regs);
  vid_selector = __dpmi_segment_to_descriptor(0xA000);
}

inline void PonPixel13(int x, int y, unsigned char c)
{
  if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
    _farpokeb(vid_selector,(y << 6) + (y << 8) + x,c);
}

inline unsigned char CogePixel13(int x, int y)
{
  if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
    return _farpeekb(vid_selector,(y << 6) + (y << 8) + x);
  else return 0;
}

inline void PonPixel13(int x, int y, unsigned char c, unsigned char *p)
{
  if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
    *(p + (y << 6) + (y << 8) + x) = c;
}

inline unsigned char CogePixel13(int x, int y, unsigned char *p)
{
  if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
    return *(p + (y << 6) + (y << 8) + x);
  else return 0;
}

void Linea(int x1, int y1, int x2, int y2, unsigned char col)
{
int dx,dy;
register int c,x,y;
register int addr;
register short int sx,bp;

  if (y1 > y2) {  // Siempre pinta en la misma direcci¢n.
    swp(y1,y2,dx);
    swp(x1,x2,dx);
  }

  dx = x2 - x1;
  dy = y2 - y1;

  if (dx > 0) {
    sx = 1;
    bp = 1;
    dx++;
  } else {
    sx = -1;
    bp = -1;
    dx = -dx + 1;
  }

  x = x1;
  y = y1;
  addr = (y << 6) + (y << 8) + x;

  for (c = dy++; y <= y2; y++) {

     if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
       _farpokeb(vid_selector,addr,col);
     if ((c -= dx) < 0) while ((addr += bp) && (x += sx) && ((c += dy) < 0))
       if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
         _farpokeb(vid_selector,addr,col);
     addr += 320;
  }
}

void Linea(int x1, int y1, int x2, int y2, unsigned char col, unsigned char *p)
{
int dx,dy;
register int c,x,y;
register int addr;
register short int sx,bp;

  if (y1 > y2) {  // Siempre pinta en la misma direcci¢n.
    swp(y1,y2,dx);
    swp(x1,x2,dx);
  }

  dx = x2 - x1;
  dy = y2 - y1;

  if (dx > 0) {
    sx = 1;
    bp = 1;
    dx++;
  } else {
    sx = -1;
    bp = -1;
    dx = -dx + 1;
  }

  x = x1;
  y = y1;
  addr = (y << 6) + (y << 8) + x;

  for (c = dy++; y <= y2; y++) {

     if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
       *(p + addr) = col;
     if ((c -= dx) < 0) while ((addr += bp) && (x += sx) && ((c += dy) < 0))
       if ((x >= 0) && (x < 320) && (y >= 0) && (y < 200))
         *(p + addr) = col;
     addr += 320;
  }
}

void Circulo(int x, int y, int r, unsigned char col)
{
 int i,j,v;

  j = r;
  v = r;

  for (i = 0; i < j; i++) {

	if (v < 0) {
	  j--;
	  v += j + j + 1;
	}

	PonPixel13(x + i,y - j,col);
	PonPixel13(x - i,y - j,col);
	PonPixel13(x + i,y + j,col);
	PonPixel13(x - i,y + j,col);

	PonPixel13(x - j,y + i,col);
	PonPixel13(x - j,y - i,col);
	PonPixel13(x + j,y + i,col);
	PonPixel13(x + j,y - i,col);

	v -= 1 + i + i;
  }
}

void Circulo(int x, int y, int r, unsigned char col, unsigned char *p)
{
 int i,j,v;

  j = r;
  v = r;

  for (i = 0; i < j; i++) {

	if (v < 0) {
	  j--;
	  v += j + j + 1;
	}

	PonPixel13(x + i,y - j,col,p);
	PonPixel13(x - i,y - j,col,p);
	PonPixel13(x + i,y + j,col,p);
	PonPixel13(x - i,y + j,col,p);

	PonPixel13(x - j,y + i,col,p);
	PonPixel13(x - j,y - i,col,p);
	PonPixel13(x + j,y + i,col,p);
	PonPixel13(x + j,y - i,col,p);

	v -= 1 + i + i;
  }
}

void PonPaleta(unsigned char c, unsigned char r, unsigned char g, unsigned char b)
{
  outportb(0x3C8,c);
  outportb(0x3C9,r);
  outportb(0x3C9,g);
  outportb(0x3C9,b);
}

void CogePaleta(unsigned char c, unsigned char &r, unsigned char &g, unsigned char &b)
{
  outportb(0x3C7,c);
  r = inportb(0x3C9);
  g = inportb(0x3C9);
  b = inportb(0x3C9);
}

void Flip(long int *p)
{
 int i;

  for (i = 0; i < 64000; i += 4) _farpokel(vid_selector,i,*p++);
}

void Retrazo()
{
  while ((inportb(0x3DA) & 8) == 0);
}

void PonPcx(char *cualo,unsigned char *donde)
{
 unsigned char *buffer;
 unsigned int ii,jj,kk,nbytes;
 unsigned char a,b;
 FILE *Fil;

   buffer = (unsigned char *) malloc(65536);

   if ((Fil = fopen(cualo,"rb")) == NULL) {
     perror("Imposible abrir el fichero ");
     exit(1);
   }

   nbytes = filelength(fileno(Fil));
   fread(buffer,1,65536,Fil);
   fclose(Fil);

   for (ii = 0; ii <= 255; ii++)
     PonPaleta(ii,buffer[nbytes - 768 + 3 * ii] >> 2,
                  buffer[nbytes - 767 + 3 * ii] >> 2,
                  buffer[nbytes - 766 + 3 * ii] >> 2);

   kk = 128;
   jj = 0;
   nbytes -= 768;

   while (kk <= nbytes) {
     a = buffer[kk++];
     if (a > 192) {
       b = a - 192;
       a = buffer[kk++];
     }
     else b = 1;

     while (b--) donde[jj++] = a;
   }

   free(buffer);
}

#endif
