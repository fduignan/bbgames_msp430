#ifndef __DISPLAY_H
#define __DISPLAY_H
#include <stdint.h>
#include "font5x7.h"
// Reset code and openAperture code comes from a mix of supplier code
// (Aliexpress) and UTFT library from rinkydink electronics

// April 2017: New I/O configuration to reduce wiring complexity
// September 2017 : Decided to control reset using P1_0
/*
   DB7 to DB3  :  P1_1 to P1_5 (note reverse order)
   DB2 to DB0  :  P2_0 to P2_2 (note reverse order)
   WR to P2_5
   RS to P2_4
   CS to P2_3
   RD to P1_7
   RST to P1_0 :
*/
#define WR_PIN P2_5
#define RS_PIN P2_4
#define CS_PIN P2_3
#define RD_PIN P1_7
#define RST_PIN P1_0
// Assumption: Top left of display is 0,0
// Screen is 176 wide and 220 high
#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220
// Define a macro to allow easy definition of colours
// Format of colour value: <BGND 1 bit><Red 5 bits><Green 5 bits><Blue 5 bits>
#define RGBToWord( R,  G,  B)  (  ((R&0xf8) << (11-3)) | ((G&0xfc) << (5-2)) | ((B&0xf8)>>3) )
// Define the primary colours
#define BGND RGBToWord(0,0,0)
#define RED RGBToWord(0xff,0,0)
#define GRN RGBToWord(0,0xff,0)
#define BLUE RGBToWord(0x1f,0x1f,0xff)
#define WHI RGBToWord(0xff,0xff,0xff)
#define YEL RGBToWord(0xff,0xff,0x00)


class Display {
  public:
    // It would have been nice to initialize the display in the contructor but this doesn't seem
    // to work as system initialization (clocks, watchdog etc) happens AFTER constructor initialization
    // A "begin" function must be called during setup to start the display up properly
    Display() {};
    void begin();
    void putPixel(uint16_t x, uint16_t y, uint16_t colour);
    void putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image);
    void fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t Colour);
    void putText(const char *Text, uint16_t len, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
    void putNumber(uint16_t Number, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);

  private:
    void openAperture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    uint16_t TextBox[FONT_WIDTH * FONT_HEIGHT];
    void output_byte(uint8_t b);
    void Write_Command(uint16_t cmd);
    void Write_Data(uint16_t data);
    void Write_Command_Data(uint16_t cmd, uint16_t dat);
    inline void WRLow()
    {
      P2OUT &= ~BIT5; // digitalWrite is too slow here
    }
    inline void WRHigh()
    {
      P2OUT |= BIT5; // digitalWrite is too slow here
    }
    inline void CommandMode()
    {
      P2OUT &= ~BIT4; // digitalWrite is too slow here
    }
    inline void DataMode()
    {
      P2OUT |= BIT4; // digitalWrite is too slow here
    }
    inline void CSLow()
    {
      digitalWrite(CS_PIN, LOW);
    }
    inline void CSHigh()
    {
      digitalWrite(CS_PIN, HIGH);
    }
    inline void RDLow()
    {
      digitalWrite(RD_PIN, LOW);
    }
    inline void RDHigh()
    {
      digitalWrite(RD_PIN, HIGH);
    }

    inline void RSTLow()
    {
      digitalWrite(RST_PIN, LOW);
    }
    inline void RSTHigh()
    {
      digitalWrite(RST_PIN, HIGH);
    }

};


void Display::begin()
{
  /* Wiring
      CS  P1.3
      RS  P1.4
      WR  P1.5
      RD  P1.6
      RST P1.7
      PORT2 = D0 to D7
  */

  pinMode(WR_PIN, OUTPUT);
  pinMode(RS_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(RD_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(P1_1, OUTPUT);
  pinMode(P1_2, OUTPUT);
  pinMode(P1_3, OUTPUT);
  pinMode(P1_4, OUTPUT);
  pinMode(P1_5, OUTPUT);
  pinMode(P2_0, OUTPUT);
  pinMode(P2_1, OUTPUT);
  pinMode(P2_2, OUTPUT);
  RSTLow();
  delay(10);
  RSTHigh();
  delay(100);
  RDHigh();
  CSHigh();
  WRHigh();
  delay(100);
  CSLow();
  // Power control startup sequence
  delay(150);
  //*********************Initial code******************************
  Write_Command_Data(0x0007, 0x0000);
  Write_Command_Data(0x0012, 0x0000);
  Write_Command_Data(0x0013, 0x0000);
  Write_Command_Data(0x0014, 0x0331);
  delay(50);

  Write_Command_Data(0x0011, 0x0101);
  Write_Command_Data(0x0012, 0x000A);
  Write_Command_Data(0x0013, 0x0D45);
  Write_Command_Data(0x0010, 0x0164);
  Write_Command_Data(0x0012, 0x001A);
  delay(50);
  Write_Command_Data(0x0013, 0x2D45);
  Write_Command_Data(0x0010, 0x0160);
  delay(50);
  Write_Command_Data(0x0060, 0x2000);
  Write_Command_Data(0x0060, 0x0000);
  Write_Command_Data(0x0061, 0x0002);
  delay(50);
  Write_Command_Data(0x0001, 0x011B);
  Write_Command_Data(0x0002, 0x0700);
  Write_Command_Data(0x0003, 0x1030);
  Write_Command_Data(0x0008, 0x0504);
  Write_Command_Data(0x0009, 0x0000);
  Write_Command_Data(0x000B, 0x0C30);
  Write_Command_Data(0x0021, 0x0000);
  Write_Command_Data(0x0030, 0x0606);
  Write_Command_Data(0x0031, 0x0606);
  Write_Command_Data(0x0032, 0x0606);
  Write_Command_Data(0x0033, 0x0803);
  Write_Command_Data(0x0034, 0x0202);
  Write_Command_Data(0x0035, 0x0404);
  Write_Command_Data(0x0036, 0x0404);
  Write_Command_Data(0x0037, 0x0404);
  Write_Command_Data(0x0038, 0x0402);
  Write_Command_Data(0x0039, 0x100C);
  Write_Command_Data(0x003A, 0x0101);
  Write_Command_Data(0x003B, 0x0101);
  Write_Command_Data(0x003C, 0x0101);
  Write_Command_Data(0x003D, 0x0101);
  Write_Command_Data(0x003E, 0x0001);
  Write_Command_Data(0x003F, 0x0001);
  Write_Command_Data(0x0042, 0xDB00);
  Write_Command_Data(0x0044, 0xAF00);
  Write_Command_Data(0x0045, 0xDB00);
  delay(50);
  Write_Command_Data(0x0071, 0x0040);
  Write_Command_Data(0x0072, 0x0002);
  delay(50);
  Write_Command_Data(0x0010, 0x6060);
  Write_Command_Data(0x0007, 0x0005);
  delay(50);
  Write_Command_Data(0x0007, 0x0025);
  Write_Command_Data(0x0007, 0x0027);
  delay(50);
  Write_Command_Data(0x0007, 0x0037);
  Write_Command_Data(0x0021, 0x0000);

  Write_Command(0x22);
  fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x00);


}
void Display::putPixel(uint16_t x, uint16_t y, uint16_t colour)
{
  openAperture(x, y, x + 1, y + 1);
  Write_Data(colour >> 8);
  Write_Data(colour & 0xff);
}
void Display::putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image)
{
  uint16_t Colour;
  openAperture(x, y, x + width - 1, y + height - 1);
  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      Colour = *(Image++);
      Write_Data(Colour);
    }
  }
}
void Display::fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t Colour)
{
  openAperture(x, y, x + width - 1, y + height - 1);
  for (y = 0; y < height; y++)
    for (x = 0; x < width; x++)
    {
      Write_Data(Colour);
    }
}
void Display::putText(const char *Text, uint16_t len, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour)
{
  uint8_t Index = 0;
  uint8_t Row, Col;
  const uint8_t *CharacterCode = 0;

  for (Index = 0; Index < len; Index++)
  {
    CharacterCode = &Font5x7[FONT_WIDTH * (Text[Index] - 32)];
    Col = 0;
    while (Col < FONT_WIDTH)
    {
      Row = 0;
      while (Row < FONT_HEIGHT)
      {
        if (CharacterCode[Col] & (1 << Row))
        {
          TextBox[(Row * FONT_WIDTH) + Col] = ForeColour;
        }
        else
        {
          TextBox[(Row * FONT_WIDTH) + Col] = BackColour;
        }
        Row++;
      }
      Col++;
    }
    putImage(x, y, FONT_WIDTH, FONT_HEIGHT, (const uint16_t *)TextBox);
    x = x + FONT_HEIGHT + 2;
  }
}
void Display::putNumber(uint16_t Number, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour)
{
  char Buffer[5]; // Maximum value = 65535
  Buffer[4] = Number % 10 + '0';
  Number = Number / 10;
  Buffer[3] = Number % 10 + '0';
  Number = Number / 10;
  Buffer[2] = Number % 10 + '0';
  Number = Number / 10;
  Buffer[1] = Number % 10 + '0';
  Number = Number / 10;
  Buffer[0] = Number % 10 + '0';
  putText(Buffer, 5, x, y, ForeColour, BackColour);
}
void Display::output_byte(uint8_t b)
{
  P1OUT &= 0xc1;
  P2OUT &= 0xf8;
  if (b)
  { // only write out bits if b is non zero to save time
    P1OUT |= reverse_bits[ (b >> 3) ] >> 2;
    P2OUT |= reverse_bits[ (b & 0x7) ] >> 5;
  }
}
void Display::Write_Command(uint16_t cmd)
{
  CommandMode();
  output_byte(cmd >> 8);
  WRLow();
  WRHigh();
  output_byte(cmd);
  WRLow();
  WRHigh();
}
void Display::Write_Data(uint16_t data)
{
  DataMode();
  output_byte(data >> 8);
  WRLow();
  WRHigh();
  output_byte(data);
  WRLow();
  WRHigh();
}
void Display::Write_Command_Data(uint16_t cmd, uint16_t dat)
{
  Write_Command(cmd);
  Write_Data(dat);
}
void Display::openAperture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  // open up an area for drawing on the display
  Write_Command_Data(0x44, (x2<<8)+x1); 
  Write_Command_Data(0x45, (y2<<8)+y1); 
  Write_Command_Data(0x21, (y1<<8)+x1); 
  Write_Command(0x22);
}

Display Screen;
#endif
