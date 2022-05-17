#include "glcdfont.h"
#include "T6963C.h"
#include <stdlib.h>

#if !defined(GRAPHIC_H)
#define GRAPHIC_H

void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char c);
void GLCD_Rectangle_Fill(unsigned char x, unsigned char y, unsigned char b, unsigned char a, unsigned char c);
void GLCD_Circle(unsigned char cx, unsigned char cy ,unsigned char radius);
void GLCD_Line(int X1, int Y1,int X2,int Y2);
void GLCD_DrawChar(int x, int y, unsigned char c, unsigned char size);
void GLCD_DrawString(int x, int y, char * str, unsigned char size);
void GLCD_DrawInt(int x, int y, int i, unsigned char size);
#endif