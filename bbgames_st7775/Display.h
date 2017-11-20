#ifndef __DISPLAY_H
#define __DISPLAY_H
#include <stdint.h>
#include "font5x7.h"
// Reset code and openAperture code comes from a mix of supplier code
// (Aliexpress) and UTFT library from rinkydink electronics

// April 2017: New I/O configuration to reduce wiring complexity
/*
   DB7 to DB3  :  P1_1 to P1_5 (note reverse order)
   DB2 to DB0  :  P2_0 to P2_2 (note reverse order)
   WR to P2_5
   RS to P2_4
   CS to P2_3
   RD to P1_7
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
    void gotoXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
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
  delay(10);
  // Power control startup sequence
  
  Write_Command_Data(0x01, 0x011C);
  Write_Command_Data(0x02, 0x0100);
  Write_Command_Data(0x03, 0x1030);
  Write_Command_Data(0x08, 0x0808);
  Write_Command_Data(0x0C, 0x0000);
  Write_Command_Data(0x0F, 0x0E01);

  Write_Command_Data(0x10, 0x0A00);
  Write_Command_Data(0x11, 0x1038);
  Write_Command_Data(0xFF, 0x0003);
  Write_Command_Data(0xB0, 0x1411);
  Write_Command_Data(0xB1, 0x0202);
  Write_Command_Data(0xB2, 0x0313);

  Write_Command_Data(0x30, 0x0000);
  Write_Command_Data(0x31, 0x00db);
  Write_Command_Data(0x32, 0x0000);
  Write_Command_Data(0x33, 0x0000);
  Write_Command_Data(0x34, 0x00db);
  Write_Command_Data(0x35, 0x0000);
  Write_Command_Data(0x36, 0x00AF);
  Write_Command_Data(0x37, 0x0000);
  Write_Command_Data(0x38, 0x00DB);
  Write_Command_Data(0x39, 0x0000);

  Write_Command_Data(0xFF, 0x0003);
  Write_Command_Data(0x50, 0x0000);
  Write_Command_Data(0x51, 0x0300);
  Write_Command_Data(0x52, 0x0103);
  Write_Command_Data(0x53, 0x2011);
  Write_Command_Data(0x54, 0x0703);
  Write_Command_Data(0x55, 0x0000);
  Write_Command_Data(0x56, 0x0400);
  Write_Command_Data(0x57, 0x0107);
  Write_Command_Data(0x58, 0x2011);
  Write_Command_Data(0x59, 0x0703);
  Write_Command_Data(0x20, 0x0000);
  Write_Command_Data(0x21, 0x0000);
  Write_Command_Data(0x07, 0x1017);
  Write_Command(0x22);
  fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x00);


}
void Display::putPixel(uint16_t x, uint16_t y, uint16_t colour)
{
  gotoXY(x, y, x + 1, y + 1);
  Write_Data(colour >> 8);
  Write_Data(colour & 0xff);
}
void Display::putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image)
{
  uint16_t Colour;
  gotoXY(x, y, x + width - 1, y + height - 1);
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
  gotoXY(x, y, x + width - 1, y + height - 1);
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
void Display::gotoXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  // open up an area for drawing on the display
  /*Write_Command_Data(0x46, (x2 << 8) | x1);
    Write_Command_Data(0x47, y2);
    Write_Command_Data(0x48, y1);*/
  Write_Command_Data(0x37, x1);
  Write_Command_Data(0x36, x2);
  Write_Command_Data(0x39, y1);
  Write_Command_Data(0x38, y2);
  Write_Command_Data(0x20, x1);
  Write_Command_Data(0x21, y1);
  Write_Command(0x22);
}

Display Screen;
#endif

