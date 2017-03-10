/*
 * fbutils.h
 *
 * Headers for utility routines for framebuffer interaction
 *
 */

#ifndef _FBUTILS_H
#define _FBUTILS_H


/* This constant, being ORed with the color index tells the library
 * to draw in exclusive-or mode (that is, drawing the same second time
 * in the same place will remove the element leaving the background intact).
 */
#define XORMODE	0x80000000

#define BLACK       0
#define BLUE        1
#define GREEN       2
#define CYAN        3
#define RED         4
#define MAGENTA     5
#define BROWN       6
#define DARKGRAY    7
#define LIGHTGRAY   8
#define LIGHTBLUE   9
#define LIGHTGREEN  10
#define LIGHTCYAN   11
#define LIGHTRED    12
#define LIGHTMAGENTA 13
#define YELLOW      14
#define WHITE       15

extern int xres, yres, fbpp, line_length;
extern char *fbptr;

int fb_init(void);

void put_cross(int x, int y, unsigned colidx);
void put_string(int x, int y, char *s);
void put_string_center(int x, int y, char *s, unsigned colidx);
void pixel (int x, int y, unsigned colidx);
void line (int x1, int y1, int x2, int y2, unsigned colidx);
void rect (int x1, int y1, int x2, int y2, unsigned colidx);
void box  (int x1, int y1, int x2, int y2, unsigned colidx);
void fb_clear(unsigned colidx);
int Get_Key(unsigned char *keyc);

#endif /* _FBUTILS_H */
