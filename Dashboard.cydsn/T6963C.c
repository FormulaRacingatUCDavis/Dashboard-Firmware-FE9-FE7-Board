// Wyświetlacz graficzny ze sterownikiem Toshiba T6963C
// (c) Radosław Kwiecień, 2007
// Kompilator : arm-elf-gcc

#include "T6963C.h"

//   PIN_DM_ALG_HIZ        Analog HiZ
//   PIN_DM_DIG_HIZ        Digital HiZ
//   PIN_DM_RES_UP        Resistive pull up
//   PIN_DM_RES_DWN        Resistive pull down
//   PIN_DM_OD_LO        Open drain - drive low
//   PIN_DM_OD_HI        Open drain - drive high
//   PIN_DM_STRONG        Strong CMOS Output
//   PIN_DM_RES_UPDWN    Resistive pull up/down

// TODO:
__inline void outByte(unsigned char byte)
{
    //AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,(0xFF << T6963_D0));
    //AT91F_PIO_ForceOutput(AT91C_BASE_PIOA, (unsigned int)(byte << T6963_D0));
    SD_SetDriveMode(PIN_DM_STRONG);
    SD_DR = SD_MASK & byte;
}


// TODO:
__inline unsigned char inByte(void)
{
    //AT91F_PIO_CfgInput(AT91C_BASE_PIOA,(0xFF << T6963_D0));
    //return ((AT91F_PIO_GetInput(AT91C_BASE_PIOA) >> T6963_D0) & 0xFF);
    SD_SetDriveMode(PIN_DM_DIG_HIZ);
    return T6963_D0_Read();
}


// funkcja wprowadzająca opóźnienie zapewniające 
// spełnienie wymagań czasowych sterownika T6963C

void delay(void)
{
    //volatile int i;
    //for(i = 0; i < 4; i++);
}



// funkcja odczytująca bajt statusu wyświetlacza
// TODO:
int GLCD_Check_Status(void)
{
    int tmp;

    //AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, T6963_CE | T6963_RD);
    CTRL_DR &= (CTRL_MASK & ~(T6963_CE | T6963_RD));
    delay();

    tmp = inByte();

    //AT91F_PIO_SetOutput(AT91C_BASE_PIOA, T6963_CE | T6963_RD);
    CTRL_DR |= (CTRL_MASK & (T6963_CE | T6963_RD));
    
    return tmp;
}

// funkcja zapisu rozkazu do sterownika
// TODO:
void GLCD_Write_Command(int command)
{
    while(!(GLCD_Check_Status()&0x03));
    
    outByte(command);
    //AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, T6963_CE | T6963_WR);
    CTRL_DR &= (CTRL_MASK & ~(T6963_CE | T6963_WR));
    
    delay();
    
    //AT91F_PIO_SetOutput(AT91C_BASE_PIOA, T6963_CE | T6963_WR);
    CTRL_DR |= (CTRL_MASK & (T6963_CE | T6963_WR));
}

// TODO:
void GLCD_Write_Data(int data)
{
    while(!(GLCD_Check_Status()&0x03));
    
    outByte(data);
    //AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, T6963_CE | T6963_WR | T6963_CD);
    CTRL_DR &= (CTRL_MASK & ~(T6963_CE | T6963_WR | T6963_CD));
    
    delay();
    
    //AT91F_PIO_SetOutput(AT91C_BASE_PIOA, T6963_CE | T6963_WR | T6963_CD);
    CTRL_DR |= (CTRL_MASK & (T6963_CE | T6963_WR | T6963_CD));
}
// TODO:
int GLCD_Read_Data(void)
{
    int tmp;
    while(!(GLCD_Check_Status()&0x03));

    //AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, T6963_CE | T6963_RD | T6963_CD);
    CTRL_DR &= (CTRL_MASK & ~(T6963_CE | T6963_RD | T6963_CD));

    delay();
    tmp = inByte();
    //AT91F_PIO_SetOutput(AT91C_BASE_PIOA, T6963_CE | T6963_RD | T6963_CD);
    CTRL_DR |= (CTRL_MASK & (T6963_CE | T6963_RD | T6963_CD));
    
    return tmp;
}

void GLCD_Clear_Text(void)
{
    int i;
    GLCD_Write_Data(GLCD_TEXT_HOME);
    GLCD_Write_Data(GLCD_TEXT_HOME >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    for(i = 0; i < GLCD_TEXT_SIZE; i++) {
        GLCD_Write_Data(0);
        GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
    }
}

void GLCD_Clear_CG(void)
{
    int i;
    GLCD_Write_Data(GLCD_EXTERNAL_CG_HOME & 0xFF);
    GLCD_Write_Data(GLCD_EXTERNAL_CG_HOME >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    for(i = 0; i < 256 * 8; i++) {
        GLCD_Write_Data(0);
        GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
    }
}


void GLCD_Clear_Graphic(void)
{
    int i;
    GLCD_Write_Data(GLCD_GRAPHIC_HOME & 0xFF);
    GLCD_Write_Data(GLCD_GRAPHIC_HOME >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    for(i = 0; i < GLCD_GRAPHIC_SIZE; i++) {
        GLCD_Write_Data(0x00);
        GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
    }
}

void GLCD_Clear_Area(unsigned char x, unsigned char y, unsigned char l, unsigned char h)
{
    int i, j, m, n;
    int x_head = (GLCD_FONT_WIDTH - (x%GLCD_FONT_WIDTH));
    int x_tail = (x+l)%GLCD_FONT_WIDTH;
    int l_whole = (l - x_head - x_tail);
    int l_font = l_whole/GLCD_FONT_WIDTH;
    int p_font = GLCD_PIXELS_PER_LINE/GLCD_FONT_WIDTH;
    
    uint16_t addr = GLCD_GRAPHIC_HOME + (p_font*y) + (x/GLCD_FONT_WIDTH);
    
    GLCD_Write_Data(addr & 0xFF);
    GLCD_Write_Data(addr >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    for (j = 0; j < h; j++) {
        for (m = 0; m < x_head; m++){
            GLCD_SetPixel(x+m,j+y,0);
        }
        
        for(i = 0; i < l_font; i++) {
            GLCD_Write_Data(0x00);
            GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
        }
        
        for (n = 0; n < x_tail; n++) {
            GLCD_SetPixel(x+x_head+l_whole+n,j+y,0);
        }
        addr += p_font;
        GLCD_Write_Data(addr & 0xFF);
        GLCD_Write_Data(addr >> 8);
        GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);
    }
}

void GLCD_Clear_Frame(void)
{
    int i;
    for(i = 0; i < GLCD_GRAPHIC_SIZE; i++) {
        FRAME[i] = 0x00;
    }
}

void GLCD_Write_Frame(void)
{
    int i;
    GLCD_Write_Data(GLCD_GRAPHIC_HOME & 0xFF);
    GLCD_Write_Data(GLCD_GRAPHIC_HOME >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    for(i = 0; i < GLCD_GRAPHIC_SIZE; i++) {
        GLCD_Write_Data(FRAME[i]);
        GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
    }
}

void GLCD_Write_Char(char ch)
{
    GLCD_Write_Data(ch - 32);
    GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
}

void GLCD_Write_String(char * str)
{
    while(*str) {
        GLCD_Write_Char(*str++);
    }
}

void GLCD_Text_GoTo(int x, int y)
{
    int address;

    address = GLCD_TEXT_HOME +  x + (GLCD_TEXT_AREA * y);

    GLCD_Write_Data(address);
    GLCD_Write_Data(address >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);
}

void GLCD_Define_Character(int charCode, int * defChar)
{
    int address;
    int i; 

    address = GLCD_EXTERNAL_CG_HOME + (8 * charCode);

    GLCD_Write_Data(address);
    GLCD_Write_Data(address >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    for(i = 0; i < 8 ; i++) {
        GLCD_Write_Data(*(defChar + i));
        GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
    }
}

// TODO:
void GLCD_Initalize(void)
{
    volatile int i;
    //GLCD_CTRL_PORT &= ~(1 << GLCD_RESET);
    //_delay_ms(1);
    //GLCD_CTRL_PORT |= (1 << GLCD_RESET);
    
    //AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, (1 << AT91C_ID_PIOA));

    //AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, (T6963_RD | T6963_WR | T6963_CD| T6963_CE|T6963_FS | T6963_RESET));
    //AT91F_PIO_SetOutput(AT91C_BASE_PIOA, (T6963_RD | T6963_WR | T6963_CD| T6963_CE|T6963_FS | T6963_RESET));
    
    CTRL_DR |= (CTRL_MASK & (T6963_RD | T6963_WR | T6963_CD| T6963_CE | T6963_FS | T6963_RESET));

    //AT91F_PIO_OutputWriteEnable(AT91C_BASE_PIOA,(0xFF << T6963_D0));

    //AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, T6963_RESET);
    CTRL_DR &= (CTRL_MASK & ~(T6963_RESET));
    for(i = 0; i < 1000; i++);
    //AT91F_PIO_SetOutput(AT91C_BASE_PIOA, T6963_RESET);
    CTRL_DR |= (CTRL_MASK & (T6963_RESET));

    #if (GLCD_FONT_WIDTH == 8)
    // AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, T6963_FS);
    CTRL_DR &= (CTRL_MASK & ~(T6963_FS));
    #endif

    GLCD_Write_Data(GLCD_GRAPHIC_HOME & 0xFF);
    GLCD_Write_Data(GLCD_GRAPHIC_HOME >> 8);
    GLCD_Write_Command(T6963_SET_GRAPHIC_HOME_ADDRESS);


    GLCD_Write_Data(GLCD_GRAPHIC_AREA);
    GLCD_Write_Data(0x00);
    GLCD_Write_Command(T6963_SET_GRAPHIC_AREA);


    GLCD_Write_Data(GLCD_TEXT_HOME);
    GLCD_Write_Data(GLCD_TEXT_HOME >> 8);
    GLCD_Write_Command(T6963_SET_TEXT_HOME_ADDRESS);

    GLCD_Write_Data(GLCD_TEXT_AREA);
    GLCD_Write_Data(0x00);
    GLCD_Write_Command(T6963_SET_TEXT_AREA);

    GLCD_Write_Data(GLCD_OFFSET_REGISTER);
    GLCD_Write_Data(0x00);
    GLCD_Write_Command(T6963_SET_OFFSET_REGISTER);


    GLCD_Write_Data(0);
    GLCD_Write_Data(0);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    GLCD_Write_Command(T6963_DISPLAY_MODE  | T6963_GRAPHIC_DISPLAY_ON   | T6963_TEXT_DISPLAY_ON /*| T6963_CURSOR_DISPLAY_ON*/);

    GLCD_Write_Command(T6963_MODE_SET | 0);

}

void GLCD_SetPixel(int x, int y, int color)
{
    uint8_t tmp;
    int byte;
    
    byte = (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
    tmp = FRAME[byte];
    
    
    if (color) 
        FRAME[byte] = (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH))) | tmp;
    else
        FRAME[byte] = ~(1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH))) & tmp;
        
}

/*
void GLCD_SetPixel(int x, int y, int color)
{
    int tmp;
    int address;
    
    address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);

    GLCD_Write_Data(address & 0xFF);
    GLCD_Write_Data(address >> 8);
    GLCD_Write_Command(T6963_SET_ADDRESS_POINTER);

    GLCD_Write_Command(T6963_DATA_READ_AND_NONVARIABLE);
    tmp = GLCD_Read_Data();

    if(color) {
      tmp |= (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
    } else {
     tmp &= ~(1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
    }

    GLCD_Write_Data(tmp);
    GLCD_Write_Command(T6963_DATA_WRITE_AND_INCREMENT);
}
*/
