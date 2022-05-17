/*
 * TFT_Library.c
 *
 *  Created: 14.10.2013 12:47:54
 *   Author: Ovner
 *
 * My additions: 9.02.2014
 */ 

#include "TFT_Library.h"
#include "fonts.h"
#include "images.h"
#include <stdio.h>
#include <string.h>

/* Variables going to extern */
uint16_t MAX_X=320, MAX_Y = 240;


//************************************************************************************
void libTFT_Start_TFT(void)
{ 
    PWM_BLED_Start();    
    TFT_Start();
    libTFT_Init(1);    // landscape with connector on right
    libTFT_ClearScreen();
}
//************************************************************************************
uint16_t constrain(uint16_t a, uint16_t b, uint16_t c)
{
	if (a < b)  { return b; }
	if (c < a)	{ return c;	}
	else return a;
}
//*************************************************************************************
void libTFT_SendCMD(uint16_t cmd)
{ 
    TFT_Write16(0,cmd);
}
//*************************************************************************************
void libTFT_WriteData(uint16_t Data)
{ 
    TFT_Write16(1,Data);
}
//*************************************************************************************
void libTFT_WriteMData(uint16_t *data, uint16 count)
{ 
//    TFT_Write8(1,Data);
    TFT_WriteM16(1, data, count);
}
//*************************************************************************************
void libTFT_SendData(uint16_t Data)
{
    /*
	uint8_t data1 = Data>>8;
	uint8_t data2 = Data&0xff;
	libTFT_WriteData(data1);
	libTFT_WriteData(data2);
    */
    libTFT_WriteData(Data);
}
//*************************************************************************************
void libTFT_Init(uint8_t orient)
{
// Soft Reset
libTFT_SendCMD  (0x01);
CyDelay(120);       // as per manual next sleep out command only allowed after 120ms, all other commands after 5ms.

//************* Start Initial Sequence **********//
//Power control A
libTFT_SendCMD  (0xCB);
libTFT_WriteData(0x39);
libTFT_WriteData(0x2C);
libTFT_WriteData(0x00);
libTFT_WriteData(0x34);
libTFT_WriteData(0x02);

//Power control A
libTFT_SendCMD  (0xCF);
libTFT_WriteData(0x00);
libTFT_WriteData(0XC1);
libTFT_WriteData(0X30);

//Driver timing control A
libTFT_SendCMD  (0xE8);
libTFT_WriteData(0x85);
libTFT_WriteData(0x00);
libTFT_WriteData(0x78);

//Driver timing control B
libTFT_SendCMD  (0xEA);
libTFT_WriteData(0x00);
libTFT_WriteData(0x00);

//Power on sequence control
libTFT_SendCMD(0xED);
libTFT_WriteData(0x64);
libTFT_WriteData(0x03);
libTFT_WriteData(0X12);
libTFT_WriteData(0X81);

//Pump ratio control
libTFT_SendCMD (0xF7);
libTFT_WriteData(0x20);

//Power Control 1
libTFT_SendCMD  (0xC0);
libTFT_WriteData(0x23);

//Power Control 2
libTFT_SendCMD  (0xC1);
libTFT_WriteData(0x10);

//VCOM Control 1
libTFT_SendCMD(0xC5);
libTFT_WriteData(0x2B);
libTFT_WriteData(0x2B);

//Memory Access Control
libTFT_SetOrientation(orient);

//Frame Rate Control (In Normal Mode/Full Colors)
libTFT_SendCMD(0xB1);
libTFT_WriteData(0x00);
libTFT_WriteData(0x1B);	// 10

//Display Function Control
libTFT_SendCMD(0xB6);
libTFT_WriteData(0x0A);
libTFT_WriteData(0x02);	// A2

//Enable 3G
libTFT_SendCMD(0xF2);
libTFT_WriteData(0x02);  //off


//COLMOD: Pixel Format Set
libTFT_SendCMD(0x3a);
libTFT_WriteData(0x05);

//Gamma Set
libTFT_SendCMD(0x26);   //Gamma curve 3
libTFT_WriteData(0x01);

//Positive Gamma Correction
libTFT_SendCMD(0xE0);
libTFT_WriteData(0x0F);
libTFT_WriteData(0x31);
libTFT_WriteData(0x2B);
libTFT_WriteData(0x0C);
libTFT_WriteData(0x0E);
libTFT_WriteData(0x08);
libTFT_WriteData(0x4E);
libTFT_WriteData(0xF1);
libTFT_WriteData(0x37);
libTFT_WriteData(0x07);
libTFT_WriteData(0x10);
libTFT_WriteData(0x03);
libTFT_WriteData(0x0E);
libTFT_WriteData(0x09);
libTFT_WriteData(0x00);

//Negative Gamma Correction
libTFT_SendCMD(0XE1);
libTFT_WriteData(0x00);
libTFT_WriteData(0x0E);
libTFT_WriteData(0x14);
libTFT_WriteData(0x03);
libTFT_WriteData(0x11);
libTFT_WriteData(0x07);
libTFT_WriteData(0x31);
libTFT_WriteData(0xC1);
libTFT_WriteData(0x48);
libTFT_WriteData(0x08);
libTFT_WriteData(0x0F);
libTFT_WriteData(0x0C);
libTFT_WriteData(0x31);
libTFT_WriteData(0x36);
libTFT_WriteData(0x0F);
// Sleep Out
libTFT_SendCMD(0x11);
CyDelay(5);        // as per manual only 5ms required
//Display On
libTFT_SendCMD(0x29);
}
//*************************************************************************************
void libTFT_SetCol(uint16_t StartCol,uint16_t EndCol)
{
	libTFT_SendCMD(0x2A);                                                      /* Column Command address       */
	libTFT_SendData(StartCol);
	libTFT_SendData(EndCol);
}
//*************************************************************************************
void libTFT_SetPage(uint16_t StartPage,uint16_t EndPage)
{
	libTFT_SendCMD(0x2B);                                                      /* Column Command address       */
	libTFT_SendData(StartPage);
	libTFT_SendData(EndPage);
}
//*************************************************************************************
void libTFT_ClearScreen(void)
{
	uint16_t i=0, total_count;
    uint16 value0[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    libTFT_SetCol(0, MAX_X);
	libTFT_SetPage(0, MAX_Y);
	libTFT_SendCMD(0x2c);                                                  /* start to write to display ra */

    total_count = MAX_X * MAX_Y / 2;
    for(i=0; i<total_count; i++)
	{
		libTFT_WriteMData(value0,4);
	}
}
//*************************************************************************************
void libTFT_FillScreen(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color)
{
	unsigned long  XY=0;
	unsigned long i=0;
	color = ~color;
	if(XL > XR)
	{
		XL = XL^XR;
		XR = XL^XR;
		XL = XL^XR;
	}
	if(YU > YD)
	{
		YU = YU^YD;
		YD = YU^YD;
		YU = YU^YD;
	}
	XL = constrain(XL, MIN_X,MAX_X);
	XR = constrain(XR, MIN_X,MAX_X);
	YU = constrain(YU, MIN_Y,MAX_Y);
	YD = constrain(YD, MIN_Y,MAX_Y);

	XY = (XR-XL+1);
	XY = XY*(YD-YU+1);

	libTFT_SetCol(XL,XR);
	libTFT_SetPage(YU, YD);
	libTFT_SendCMD(0x2c);         /* start to write to display ra */
	/* m                            */

    /*
	uint8_t Hcolor = color>>8;
	uint8_t Lcolor = color&0xff;
    */
        
	for(i=0; i < XY; i++)
	{\
        /*
		libTFT_WriteData(Hcolor);
		libTFT_WriteData(Lcolor);
        */
        libTFT_WriteData(color);
	}

}
//*************************************************************************************
void libTFT_SetXY(uint16_t poX, uint16_t poY)
{
	libTFT_SetCol(poX, poX);
	libTFT_SetPage(poY, poY);
	libTFT_SendCMD(0x2c);
}
//*************************************************************************************
void libTFT_SetPixel(uint16_t poX, uint16_t poY,uint16_t color)
{
	libTFT_SetXY(poX, poY);
	libTFT_SendData(~color);
}
//*************************************************************************************
void libTFT_FillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color)
{
	libTFT_FillScreen(poX, poX+length, poY, poY+width, color);
}
//*************************************************************************************
void  libTFT_DrawHorizontalLine( uint16_t poX, uint16_t poY, uint16_t length,uint16_t color)
{
	int i=0;
	libTFT_SetCol(poX,poX + length);
	libTFT_SetPage(poY,poY);
	libTFT_SendCMD(0x2c);
	for(i=0; i<length; i++)
	libTFT_SendData(~color);
}
//*************************************************************************************
void libTFT_DrawLine( uint16_t x0,uint16_t y0,uint16_t x1, uint16_t y1,uint16_t color)
{
	int x = x1-x0;
	int y = y1-y0;
	int dx = abs(x), sx = x0<x1 ? 1 : -1;
	int dy = -abs(y), sy = y0<y1 ? 1 : -1;
	int err = dx+dy, e2;                                                /* error value e_xy             */
	for (;;){                                                           /* loop                         */
		libTFT_SetPixel(x0,y0,color);
		e2 = 2*err;
		if (e2 >= dy) {                                                 /* e_xy+e_x > 0                 */
			if (x0 == x1) break;
			err += dy; x0 += sx;
		}
		if (e2 <= dx) {                                                 /* e_xy+e_y < 0                 */
			if (y0 == y1) break;
			err += dx; y0 += sy;
		}
	}
}
//*************************************************************************************
void libTFT_DrawVerticalLine( uint16_t poX, uint16_t poY, uint16_t length,uint16_t color)
{
	int i=0;
	libTFT_SetCol(poX,poX);
	libTFT_SetPage(poY,poY+length);
	libTFT_SendCMD(0x2c);
	for(i=0; i<length; i++)
	libTFT_SendData(~color);
}
//*************************************************************************************
void libTFT_DrawRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width,uint16_t color)
{
	libTFT_DrawHorizontalLine(poX, poY, length, color);
	libTFT_DrawHorizontalLine(poX, poY+width, length, color);
	libTFT_DrawVerticalLine(poX, poY, width,color);
	libTFT_DrawVerticalLine(poX + length, poY, width,color);
}
//*************************************************************************************
void libTFT_DrawCircle(int poX, int poY, int r,uint16_t color)
{
	int x = -r, y = 0, err = 2-2*r, e2;
	do {
		libTFT_SetPixel(poX-x, poY+y,color);
		libTFT_SetPixel(poX+x, poY+y,color);
		libTFT_SetPixel(poX+x, poY-y,color);
		libTFT_SetPixel(poX-x, poY-y,color);
		e2 = err;
		if (e2 <= y) {
			err += ++y*2+1;
			if (-x == y && e2 <= x) e2 = 0;
		}
		if (e2 > x) err += ++x*2+1;
	} while (x <= 0);
}
//*************************************************************************************
void libTFT_FillCircle(int poX, int poY, int r,uint16_t color)
{
	int x = -r, y = 0, err = 2-2*r, e2;
	do {
		libTFT_DrawVerticalLine(poX-x, poY-y, 2*y, color);
		libTFT_DrawVerticalLine(poX+x, poY-y, 2*y, color);

		e2 = err;
		if (e2 <= y) {
			err += ++y*2+1;
			if (-x == y && e2 <= x) e2 = 0;
		}
		if (e2 > x) err += ++x*2+1;
	} while (x <= 0);
}
//*************************************************************************************
void libTFT_SetOrientation(uint8_t orient)
{
	libTFT_SendCMD(0x36);
	switch (orient)
	{
		case 0: libTFT_WriteData(0x48); // portrait with connector bottom
				break;
		case 1: libTFT_WriteData(0x28); // landscape with connector right
				break;
		case 2: libTFT_WriteData(0x88); // portrait with connector top
				break;
		case 3: libTFT_WriteData(0xE8); // landscape with connector left
				break;
	}
	if (orient == 0 || orient == 2)
	{ MAX_X = 239; MAX_Y = 319;	}
	else
	{ MAX_X = 319;	MAX_Y = 239;}
}
//*************************************************************************************
void libTFT_DrawChar( uint8_t ascii, uint16_t poX, uint16_t poY,uint16_t size, uint16_t fgcolor, const char font[][16])
{
	int i = 0;   uint8_t f =0 ;
	uint8_t temp=0, k;
	if((ascii>=32)&&(ascii<=255))	{;}
	else	{ascii = '?'-32;}
	
	for (i = 0; i < FONT_X; i++ ) 
	{
		if ((ascii >= 0x20) && (ascii <= 0x7F))
		{
			temp = (font[ascii-0x20][i]);
		}
		else if ( ascii >= 0xC0 )
		{
			temp = (font[ascii-0x65][i]);
		}
		k=i / 8;
		for(f =0 ; f < FONT_Y; f++)
		{
			if((temp>>f)&0x01)
			{
				if (size == 0)libTFT_SetPixel(poX+i-(k*8), poY+f+(k*8),fgcolor);
				else libTFT_FillRectangle(poX+i*size-(k*8)*size, poY+f*size+(k*8)*size, size, size, fgcolor);
			}
		}
	}
}

//*************************************************************************************
void libTFT_DrawString(char *string,uint16_t poX, uint16_t poY, uint16_t size,uint16_t fgcolor, const char font[][16])
{
	while(*string)
	{
		if((poX + FONT_SPACE) > MAX_X)		{poX = 1; poY = poY + FONT_X*size;}
		
		libTFT_DrawChar(*string, poX, poY, size, fgcolor, font);
		if (size > 0) poX += FONT_SPACE*size;
		else poX += FONT_SPACE;
		string++;
	}
}

//*************************************************************************************
void libTFT_DrawInt(int i,uint16_t poX, uint16_t poY, uint16_t size,uint16_t fgcolor, const char font[][16]){
    char stringi[10];
    itoa(i, stringi, 10);
    libTFT_DrawString(stringi, poX, poY, size, fgcolor, font);
}

//************************************************************************************++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DrawBitmap -- A backup bitmap function that handels raw hexdumps of bitmaps, as opposed to whatever the one that came with this library does (it frightens me)
void libTFT_DrawBitmap(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, const char bitmap[]){
    
    //were not interested in color profile information so the first thing we need is the pixel offset.
    //for all bitmaps, pixel offset is 10 bytes in and 4 bytes long, starting at index 20 and ending at index 27.
    char parsePixelOffset[4];
    int pixelOffset;
    for(int i = 0; i < 4; i++){
        parsePixelOffset[i] = bitmap[20 + i];
    }
    pixelOffset = stoi(parsePixelOffset,nullptr,16);
    
}

void libTFT_FillBitmap(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, unsigned short *Bitmap)
{
	unsigned long  XY=0;
	unsigned long i=0;
	XY = (XR-XL+1);
	XY = XY*(YD-YU+1);

	libTFT_SetCol(XL,XR);
	libTFT_SetPage(YU, YD);
	libTFT_SendCMD(0x2c);                     /* start to write to display ra */

	for(i=0; i < XY; i++)
	{										 //color[i] =( ~color[i+1]);	
	uint8_t Hcolor = (~Bitmap[i])>>8;
	uint8_t Lcolor = (~Bitmap[i])&0xff;	
		libTFT_WriteData(Hcolor);
		libTFT_WriteData(Lcolor);
	}
}

/*

void Read_SD_Pictute(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, unsigned short *FileName)
{
char  fp;
long  XY=0, i=0;
XY = (XR-XL+1);
XY = 2*XY*(YD-YU)+1;
libTFT_SetCol(XL,XR);
libTFT_SetPage(YU, YD);
libTFT_SendCMD(0x2c);     // start to write to display ra 
fp = SDCard_fopen((uint8 *)FileName, (uint8 *)"r");

 while(! SDCard_feof(fp))           // Check for end of input file
{
extern uint8   SDCard_SectBuf1[];
extern uint8   SDCard_SectBuf2[];

  SDCard_ReadFileSect(fp);    // Read sector

	for(i=0; i < 256 ; i++)
		{	if(XY<0)  break;
		libTFT_WriteData(~SDCard_SectBuf1[i]); XY--; }

	for(i=0; i < 256; i++)
		{ 	if(XY<0)  break;
		libTFT_WriteData(~SDCard_SectBuf2[i]); XY--; }
	
SDCard_NextSect(fp);               // Advance to next read sector
}
	SDCard_fclose(fp); 
}

*/

//----------------------------------------------------------------------------------
void libTFT_Graf(uint16_t XL, uint16_t YU, uint16_t YD, char  Ydata , char last)
{
	unsigned long  XY=0;
	unsigned long i=0;
//	volatile char last;
	//XY = (XR-XL+1);
	XY = YD-YU+1;

	libTFT_SetCol(XL,XL);
	libTFT_SetPage(YU, YD);
	libTFT_SendCMD(0x2c);                     /* start to write to display ra */

	for(i=0; i < XY; i++)
	{										 //color[i] =( ~color[i+1]);	
   //  uint8_t Hcolor = 255*( (i<last )    );
	//uint8_t Hcolor = ~(255*( ((i>=last)&(i<=Ydata))|((i<=last)&(i>=Ydata))  ) );
 uint8_t Hcolor =  (255*( ((i<last)|(i>Ydata))&((i>last)|(i<Ydata))  ) );
	//uint8_t Hcolor = 255*(Ydata!=i);//(~Bitmap[i])>>8;
	uint8_t Lcolor = 128;//(~Bitmap[i])&0xff;	
		libTFT_WriteData(Hcolor);
		libTFT_WriteData(Lcolor);
		
		
	}
last =Ydata;

}

void libTFT_DrawButton(uint16 xpos,uint16 ypos,uint16 width,uint16 height,uint8 pressed,char *label, const char font[][16])
{
    uint16 bgcolor, tcolor, lpos, tpos;
    
    lpos = xpos + ((width / 2) - (8 * (1 + strlen(label)) / 2));
    tpos = ypos + ((height / 2) - 8);
    if (pressed)
    {
        bgcolor = DGRAY;
        tcolor = YELLOW;
    }else
    {
        bgcolor = LGRAY;
        tcolor = RED;
    }
    
    libTFT_DrawRectangle(xpos, ypos, width, height, BLACK);
    libTFT_FillRectangle(xpos+1, ypos+1, width+-2, height-2, bgcolor);
    libTFT_DrawString(label, lpos, tpos, 1, tcolor, font);
}

libTFT_Button *libTFT_CreateButton(uint16 xpos,uint16 ypos,uint16 width,uint16 height,char *label, const char font[][16])
{
    libTFT_Button *temp_button;
    temp_button = (libTFT_Button *) malloc(sizeof(libTFT_Button));
    if (temp_button != NULL)
    {
        temp_button->xpos = xpos;
        temp_button->ypos = ypos;
        temp_button->width = width;
        temp_button->height = height;
        temp_button->pressed = 0;
        temp_button->label = label;
        libTFT_DrawButton(temp_button->xpos, temp_button->ypos, temp_button->width, temp_button->height, temp_button->pressed, temp_button->label, font); 
    }
    return temp_button;
}