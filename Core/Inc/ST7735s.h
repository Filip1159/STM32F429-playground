#ifndef LCD_FUNC
#define LCD_FUNC

#define SWRESET 0x01 // software reset
#define SLPOUT 0x11 // sleep out
#define DISPOFF 0x28 // display off
#define DISPON 0x29 // display on
#define CASET 0x2A // column address set
#define RASET 0x2B // row address set
#define RAMWR 0x2C // RAM write
#define MADCTL 0x36 // axis control
#define COLMOD 0x3A // color mode
// 1.8" TFT display constants
#define XSIZE 128
#define YSIZE 160
#define XMAX XSIZE-1
#define YMAX YSIZE-1
// Color constants
#define BLACK 0x0000
#define BLUE 0x001F   //  0000 0000 0001 1111
#define RED 0xF800    //  1111 1000 0000 0000
#define GREEN 0x0400  //  0000 0100 0000 0000
#define LIME 0x07E0   //  0000 0111 1110 0000
#define CYAN 0x07FF   //  0000 0111 1111 1111
#define MAGENTA 0xF81F//  1111 1000 0001 1111
#define YELLOW 0xFFE0 //  1111 1111 1110 0000
#define WHITE 0xFFFF

#include "stdint.h"
#include "main.h"

class ST7735s {
private:
	uint8_t curX, curY;
	static const uint8_t FONT_CHARS[96][5];
	SPI_HandleTypeDef hspi;
	unsigned long intsqrt(unsigned long val);
	void writeCmd(uint8_t cmd);
	void writeByte(uint8_t b);
	void writeWord(uint16_t w);
	void write565(uint16_t data, unsigned int count);

public:
	ST7735s();
	ST7735s(SPI_HandleTypeDef& hspi);
	void init();
	void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void clearScreen();
	void drawPixel(uint16_t x, uint16_t y, int color);

	void horizontalLine(uint8_t x0, uint8_t x1, uint8_t y, int color);
	void verticalLine(uint8_t x, uint8_t y0, uint8_t y1, int color);
	void line(int x0, int y0, int x1, int y1, int color);
	void drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, int color);
	void fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, int color);
	void circleQuadrant(uint8_t xPos, uint8_t yPos, uint8_t radius, uint8_t quad, int color);
	void drawCircle(uint8_t xPos, uint8_t yPos, uint8_t radius, int color);
	void drawRoundRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t r, int color);
	void fillCircle(uint8_t xPos, uint8_t yPos, uint8_t radius, int color);
	void drawEllipse(int x0, int y0, int width, int height, int color);
	void fillEllipse(int xPos,int yPos,int width,int height, int color);
	void gotoXY(uint8_t x, uint8_t y);
	void gotoLine(uint8_t y);
	void advanceCursor();
	void setOrientation(int degrees);
	void putCh(char ch, uint8_t x, uint8_t y, int color);
	void writeChar(char ch, int color);
	void writeString(char *text, int color);
	void writeInt(int i);
	void writeHex(int i);
};

#endif
