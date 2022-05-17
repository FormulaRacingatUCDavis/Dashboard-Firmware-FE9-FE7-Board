/*
 * TFT_Library.h
 *  
 */ 


#ifndef libTFT_H_
#define libTFT_H_

#include <stdlib.h>
#include <stdint.h>

#include <device.h>
	

#define FONT_SPACE 10
#define FONT_X 16
#define FONT_Y 8
    
//Basic Colors
#define RED	 	0xf800
#define YELLOW	0xffe0
#define GREEN	0x07e0
#define BLUE	0x001f
#define CYAN	0x07ff
#define MAGENTA 0xF81F
#define BLACK	0x0000
#define WHITE	0xffff

//Other Colors   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
#define BRED		0xf810	
#define GRAY1		0x8410  
#define GRAY2		0x4208  
#define BLUE2    	0x051F

#define   NAVY   	0x000F
#define   DGREEN 	0x03E0
#define   DCYAN   	0x03EF
#define   MAROON  	0x7800
#define   PURPLE  	0x780F
#define   OLIVE   	0x7BE0
#define   GREY    	0xF7DE
#define   LGRAY  	0xC618
#define   DGRAY   	0x7BEF
#define   BROWN   	0xBC40 

#define MIN_X	0
#define MIN_Y	0

typedef struct {
    uint16 xpos;
    uint16 ypos;
    uint16 width;
    uint16 height;
    uint8 pressed;
    char* label;
} libTFT_Button;
    

//****************************************************************************************
void libTFT_Start_TFT(void);
void libTFT_SendCMD(uint16_t cmd);
void libTFT_WriteData(uint16_t data);
void libTFT_WriteMData(uint16_t *data, uint16 count);
void libTFT_SendData(uint16_t data);
void libTFT_Init(uint8_t orient);
void libTFT_ClearScreen(void);
void libTFT_FillScreen(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color);
void libTFT_FillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);
void libTFT_DrawHorizontalLine( uint16_t poX, uint16_t poY, uint16_t length,uint16_t color);
void libTFT_DrawLine( uint16_t x0,uint16_t y0,uint16_t x1, uint16_t y1,uint16_t color);
void libTFT_DrawVerticalLine( uint16_t poX, uint16_t poY, uint16_t length,uint16_t color);
void libTFT_DrawRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width,uint16_t color);
void libTFT_DrawCircle(int poX, int poY, int r,uint16_t color);
void libTFT_FillCircle(int poX, int poY, int r,uint16_t color);
void libTFT_SetOrientation(uint8_t orient);
void libTFT_DrawChar( uint8_t ascii, uint16_t poX, uint16_t poY,uint16_t size, uint16_t fgcolor, const char font[][16]);
void libTFT_DrawString(char *string,uint16_t poX, uint16_t poY, uint16_t size,uint16_t fgcolor, const char font[][16]);
void libTFT_DrawInt(int i,uint16_t poX, uint16_t poY, uint16_t size,uint16_t fgcolor, const char font[][16]);
void libTFT_SetPixel(uint16_t poX, uint16_t poY,uint16_t color);
//***************************************************************************************

void libTFT_DrawBitmap(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, const char bitmap[]);
void libTFT_FillBitmap(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, unsigned short *Bitmap);
void libTFT_Graf(uint16_t XL, uint16_t YU, uint16_t YD, char  Ydata , char last);
// void Read_SD_Pictute(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, unsigned short *FileName);
//void libTFT_GrafN(unsigned int Xi, unsigned int Yo, uint16_t height, uint8*  Ydata , uint8* last, uint16_t *colors, uint16_t Blcolor, uint8 Nch);
//void libTFT_DrawStringBl(char *string,uint16_t poX, uint16_t poY, uint16_t size,uint16_t fgcolor,uint16_t bgcolor);
void libTFT_DrawButton(uint16 xpos,uint16 ypos,uint16 width,uint16 height,uint8 pressed,char *label, const char font[][16]);
libTFT_Button* libTFT_CreateButton(uint16 xpos,uint16 ypos,uint16 width,uint16 height,char *label, const char font[][16]);

#endif /* libTFT_H_ */