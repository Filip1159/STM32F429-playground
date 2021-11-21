#include "ST7735s.h"
#include "math.h"
#include "stdlib.h"
#include "stdint.h"

const uint8_t ST7735s::FONT_CHARS[96][5] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00 }, // (space)
	{ 0x00, 0x00, 0x5F, 0x00, 0x00 }, // !
	{ 0x00, 0x07, 0x00, 0x07, 0x00 }, // "
	{ 0x14, 0x7F, 0x14, 0x7F, 0x14 }, // #
	{ 0x24, 0x2A, 0x7F, 0x2A, 0x12 }, // $
	{ 0x23, 0x13, 0x08, 0x64, 0x62 }, // %
	{ 0x36, 0x49, 0x55, 0x22, 0x50 }, // &
	{ 0x00, 0x05, 0x03, 0x00, 0x00 }, // '
	{ 0x00, 0x1C, 0x22, 0x41, 0x00 }, // (
	{ 0x00, 0x41, 0x22, 0x1C, 0x00 }, // )
	{ 0x08, 0x2A, 0x1C, 0x2A, 0x08 }, // *
	{ 0x08, 0x08, 0x3E, 0x08, 0x08 }, // +
	{ 0x00, 0x50, 0x30, 0x00, 0x00 }, // ,
	{ 0x08, 0x08, 0x08, 0x08, 0x08 }, // -
	{ 0x00, 0x60, 0x60, 0x00, 0x00 }, // .
	{ 0x20, 0x10, 0x08, 0x04, 0x02 }, // /
	{ 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
	{ 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
	{ 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
	{ 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 3
	{ 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
	{ 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
	{ 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
	{ 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7
	{ 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
	{ 0x06, 0x49, 0x49, 0x29, 0x1E }, // 9
	{ 0x00, 0x36, 0x36, 0x00, 0x00 }, // :
	{ 0x00, 0x56, 0x36, 0x00, 0x00 }, // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41 }, // <
	{ 0x14, 0x14, 0x14, 0x14, 0x14 }, // =
	{ 0x41, 0x22, 0x14, 0x08, 0x00 }, // >
	{ 0x02, 0x01, 0x51, 0x09, 0x06 }, // ?
	{ 0x32, 0x49, 0x79, 0x41, 0x3E }, // @
	{ 0x7E, 0x11, 0x11, 0x11, 0x7E }, // A
	{ 0x7F, 0x49, 0x49, 0x49, 0x36 }, // B
	{ 0x3E, 0x41, 0x41, 0x41, 0x22 }, // C
	{ 0x7F, 0x41, 0x41, 0x22, 0x1C }, // D
	{ 0x7F, 0x49, 0x49, 0x49, 0x41 }, // E
	{ 0x7F, 0x09, 0x09, 0x01, 0x01 }, // F
	{ 0x3E, 0x41, 0x41, 0x51, 0x32 }, // G
	{ 0x7F, 0x08, 0x08, 0x08, 0x7F }, // H
	{ 0x00, 0x41, 0x7F, 0x41, 0x00 }, // I
	{ 0x20, 0x40, 0x41, 0x3F, 0x01 }, // J
	{ 0x7F, 0x08, 0x14, 0x22, 0x41 }, // K
	{ 0x7F, 0x40, 0x40, 0x40, 0x40 }, // L
	{ 0x7F, 0x02, 0x04, 0x02, 0x7F }, // M
	{ 0x7F, 0x04, 0x08, 0x10, 0x7F }, // N
	{ 0x3E, 0x41, 0x41, 0x41, 0x3E }, // O
	{ 0x7F, 0x09, 0x09, 0x09, 0x06 }, // P
	{ 0x3E, 0x41, 0x51, 0x21, 0x5E }, // Q
	{ 0x7F, 0x09, 0x19, 0x29, 0x46 }, // R
	{ 0x46, 0x49, 0x49, 0x49, 0x31 }, // S
	{ 0x01, 0x01, 0x7F, 0x01, 0x01 }, // T
	{ 0x3F, 0x40, 0x40, 0x40, 0x3F }, // U
	{ 0x1F, 0x20, 0x40, 0x20, 0x1F }, // V
	{ 0x7F, 0x20, 0x18, 0x20, 0x7F }, // W
	{ 0x63, 0x14, 0x08, 0x14, 0x63 }, // X
	{ 0x03, 0x04, 0x78, 0x04, 0x03 }, // Y
	{ 0x61, 0x51, 0x49, 0x45, 0x43 }, // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41 }, // [
	{ 0x02, 0x04, 0x08, 0x10, 0x20 }, // "\"
	{ 0x41, 0x41, 0x7F, 0x00, 0x00 }, // ]
	{ 0x04, 0x02, 0x01, 0x02, 0x04 }, // ^
	{ 0x40, 0x40, 0x40, 0x40, 0x40 }, // _
	{ 0x00, 0x01, 0x02, 0x04, 0x00 }, // `
	{ 0x20, 0x54, 0x54, 0x54, 0x78 }, // a
	{ 0x7F, 0x48, 0x44, 0x44, 0x38 }, // b
	{ 0x38, 0x44, 0x44, 0x44, 0x20 }, // c
	{ 0x38, 0x44, 0x44, 0x48, 0x7F }, // d
	{ 0x38, 0x54, 0x54, 0x54, 0x18 }, // e
	{ 0x08, 0x7E, 0x09, 0x01, 0x02 }, // f
	{ 0x08, 0x14, 0x54, 0x54, 0x3C }, // g
	{ 0x7F, 0x08, 0x04, 0x04, 0x78 }, // h
	{ 0x00, 0x44, 0x7D, 0x40, 0x00 }, // i
	{ 0x20, 0x40, 0x44, 0x3D, 0x00 }, // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44 }, // k
	{ 0x00, 0x41, 0x7F, 0x40, 0x00 }, // l
	{ 0x7C, 0x04, 0x18, 0x04, 0x78 }, // m
	{ 0x7C, 0x08, 0x04, 0x04, 0x78 }, // n
	{ 0x38, 0x44, 0x44, 0x44, 0x38 }, // o
	{ 0x7C, 0x14, 0x14, 0x14, 0x08 }, // p
	{ 0x08, 0x14, 0x14, 0x18, 0x7C }, // q
	{ 0x7C, 0x08, 0x04, 0x04, 0x08 }, // r
	{ 0x48, 0x54, 0x54, 0x54, 0x20 }, // s
	{ 0x04, 0x3F, 0x44, 0x40, 0x20 }, // t
	{ 0x3C, 0x40, 0x40, 0x20, 0x7C }, // u
	{ 0x1C, 0x20, 0x40, 0x20, 0x1C }, // v
	{ 0x3C, 0x40, 0x30, 0x40, 0x3C }, // w
	{ 0x44, 0x28, 0x10, 0x28, 0x44 }, // x
	{ 0x0C, 0x50, 0x50, 0x50, 0x3C }, // y
	{ 0x44, 0x64, 0x54, 0x4C, 0x44 }, // z
	{ 0x00, 0x08, 0x36, 0x41, 0x00 }, // {
	{ 0x00, 0x00, 0x7F, 0x00, 0x00 }, // |
	{ 0x00, 0x41, 0x36, 0x08, 0x00 }, // }
	{ 0x08, 0x08, 0x2A, 0x1C, 0x08 }, // ->
	{ 0x08, 0x1C, 0x2A, 0x08, 0x08 }, // <-
};

ST7735s::ST7735s() {

}

ST7735s::ST7735s(SPI_HandleTypeDef& hspi) {
	this->hspi = hspi;
}

unsigned long ST7735s::intsqrt(unsigned long val) {
	unsigned long mulMask = 0x0008000;
	unsigned long retVal = 0;
	if (val > 0) {
		while (mulMask != 0) {
			retVal |= mulMask;
			if (retVal*retVal > val)
				retVal &= ~ mulMask;
			mulMask >>= 1;
		}
	}
	return retVal;
}

void ST7735s::writeCmd(uint8_t cmd) {
	HAL_GPIO_WritePin(ST7735S_DC_GPIO_Port, ST7735S_DC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi, &cmd, 1, 1000);
	HAL_GPIO_WritePin(ST7735S_DC_GPIO_Port, ST7735S_DC_Pin, GPIO_PIN_SET);
}
void ST7735s::writeByte(uint8_t b) {
	HAL_SPI_Transmit(&hspi, &b, 1, 1000);
}

void ST7735s::writeWord(uint16_t w) {
	uint8_t first = w>>8, second = w&0xFF;

	HAL_SPI_Transmit(&hspi, &first, 1, 1000);
	HAL_SPI_Transmit(&hspi, &second, 1, 1000);
}

void ST7735s::write565(uint16_t data, unsigned int count) {
	writeCmd(RAMWR);
	for (; count > 0; count--) {
		writeWord(data);
	}
}

void ST7735s::init() {
	HAL_GPIO_WritePin(ST7735S_RST_GPIO_Port, ST7735S_RST_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(ST7735S_RST_GPIO_Port, ST7735S_RST_Pin, GPIO_PIN_RESET);  // hardware reset
	HAL_Delay(1);
	HAL_GPIO_WritePin(ST7735S_RST_GPIO_Port, ST7735S_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(150);

	writeCmd(SLPOUT);
	HAL_Delay(150);
	writeCmd(COLMOD);
	writeByte(0x05); // select color mode 5 = 16bit pixels (RGB565)
	writeCmd(DISPON);
}

void ST7735s::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	writeCmd(CASET); // set column range (x0,x1)
	writeWord(x0+1);
	writeWord(x1+1);
	writeCmd(RASET); // set row range (y0,y1)
	writeWord(y0);
	writeWord(y1);
}

void ST7735s::drawPixel(uint16_t x, uint16_t y, int color) {
	setAddrWindow(x, y, x, y);
	write565(color, 1);
}

void ST7735s::horizontalLine(uint8_t x0, uint8_t x1, uint8_t y, int color) {
	uint8_t width = x1 - x0 + 1;
	setAddrWindow(x0, y, x1, y);
	write565(color, width);
}

void ST7735s::verticalLine(uint8_t x, uint8_t y0, uint8_t y1, int color) {
	uint8_t height = y1 - y0 + 1;
	setAddrWindow(x, y0, x, y1);
	write565(color, height);
}

void ST7735s::line(int x0, int y0, int x1, int y1, int color) { // Bresenham algorithm
	int dx = fabs(x1 - x0), sx = x0<x1 ? 1 : -1;
	int dy = fabs(y1 - y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;
	for(;;) {
		drawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void ST7735s::drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, int color) {
	horizontalLine(x0, x1, y0, color);
	horizontalLine(x0, x1, y1, color);
	verticalLine(x0, y0, y1, color);
	verticalLine(x1, y0, y1, color);
}

void ST7735s::fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, int color) {
	uint8_t width = x1 - x0 + 1;
	uint8_t height = y1 - y0 + 1;
	setAddrWindow(x0, y0, x1, y1);
	write565(color, width*height);
}

void ST7735s::clearScreen() {
	fillRect(0, 0, 129, 160, BLACK);
}

void ST7735s::circleQuadrant(uint8_t xPos, uint8_t yPos, uint8_t radius, uint8_t quad, int color) {
// draws circle quadrant(s) centered at x,y with given radius & color
// quad is a bit-encoded representation of which cartesian quadrant(s) to draw.
// Remember that the y axis on our display is 'upside down':
// bit 0: draw quadrant I (lower right)
// bit 1: draw quadrant IV (upper right)
// bit 2: draw quadrant II (lower left)
// bit 3: draw quadrant III (upper left)
	int x, xEnd = (707*radius)/1000 + 1;
	for (x=0; x<xEnd; x++) {
		uint8_t y = intsqrt(radius*radius - x*x);
		if (quad & 0x01) {
			drawPixel(xPos+x,yPos+y,color); // lower right
			drawPixel(xPos+y,yPos+x,color);
		}
		if (quad & 0x02) {
			drawPixel(xPos+x,yPos-y,color); // upper right
			drawPixel(xPos+y,yPos-x,color);
		}
		if (quad & 0x04) {
			drawPixel(xPos-x,yPos+y,color); // lower left
			drawPixel(xPos-y,yPos+x,color);
		}
		if (quad & 0x08) {
			drawPixel(xPos-x,yPos-y,color); // upper left
			drawPixel(xPos-y,yPos-x,color);
		}
	}
}

void ST7735s::drawCircle(uint8_t xPos, uint8_t yPos, uint8_t radius, int color) {
	circleQuadrant(xPos, yPos, radius, 0x0F, color); // do all 4 quadrants
}

void ST7735s::drawRoundRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t r, int color) {
	horizontalLine(x0+r, x1-r, y0, color); // top side
	horizontalLine(x0+r, x1-r, y1, color); // bottom side
	verticalLine(x0, y0+r, y1-r, color); // left side
	verticalLine(x1, y0+r, y1-r, color); // right side
	circleQuadrant(x0+r, y0+r, r, 8, color); // upper left corner
	circleQuadrant(x1-r, y0+r, r, 2, color); // upper right corner
	circleQuadrant(x0+r, y1-r, r, 4, color); // lower left corner
	circleQuadrant(x1-r, y1-r, r, 1, color); // lower right corner
}

void ST7735s::fillCircle(uint8_t xPos, uint8_t yPos, uint8_t radius, int color) {
	long r2 = radius * radius;
	for (int x = 0; x <= radius; x++) {
		uint8_t y = intsqrt(r2-x*x);
		uint8_t y0 = yPos - y;
		uint8_t y1 = yPos + y;
		verticalLine(xPos+x, y0, y1, color);
		verticalLine(xPos-x, y0, y1, color);
	}
}

void ST7735s::drawEllipse(int x0, int y0, int width, int height, int color) {
// draws an ellipse of given width & height
// two-part Bresenham method
// note: slight discontinuity between parts on some (narrow) ellipses.
	int a = width/2, b = height/2;
	int x = 0, y = b;
	long a2 = (long)a*a*2;
	long b2 = (long)b*b*2;
	long error = (long)a*a*b;
	long stopY = 0, stopX = a2*b;
	while (stopY <= stopX) {
		drawPixel(x0+x, y0+y, color);
		drawPixel(x0+x, y0-y, color);
		drawPixel(x0-x, y0+y, color);
		drawPixel(x0-x, y0-y, color);
		x++;
		error -= b2*(x-1);
		stopY += b2;
		if (error < 0) {
			error += a2*(y-1);
			y--;
			stopX -= a2;
		}
	}
	x = a; y = 0; error = b*b*a;
	stopY = a*b2; stopX = 0;
	while (stopY >= stopX) {
		drawPixel(x0+x, y0+y, color);
		drawPixel(x0+x, y0-y, color);
		drawPixel(x0-x, y0+y, color);
		drawPixel(x0-x, y0-y, color);
		y++;
		error -= a2*(y-1);
		stopX += a2;
		if (error < 0) {
			error += b2*(x-1);
			x--;
			stopY -= b2;
		}
	}
}

void ST7735s::fillEllipse(int xPos,int yPos,int width,int height, int color) {
	int a = width/2, b = height/2; // get x & y radii
	int x1, x0 = a, y = 1, dx = 0;
	long a2 = a*a, b2 = b*b; // need longs: big numbers!
	long a2b2 = a2 * b2;
	horizontalLine(xPos-a, xPos+a, yPos, color); // draw centerline
	while (y<=b) { // draw horizontal lines...
		for (x1= x0-(dx-1); x1>0; x1--)
			if (b2*x1*x1 + a2*y*y <= a2b2)
				break;
		dx = x0 - x1;
		x0 = x1;
		horizontalLine(xPos-x0, xPos+x0, yPos+y, color);
		horizontalLine(xPos-x0, xPos+x0, yPos-y, color);
		y++;
	}
}
// ---------------------------------------------------------------------------
// TEXT ROUTINES
//
// Each ASCII character is 5x7, with one pixel space between characters
// So, character width = 6 pixels & character height = 8 pixels.
//
// In portrait mode:
// Display width = 128 pixels, so there are 21 chars/row (21x6 = 126).
// Display height = 160 pixels, so there are 20 rows (20x8 = 160).
// Total number of characters in portait mode = 21 x 20 = 420.
//
// In landscape mode:
// Display width is 160, so there are 26 chars/row (26x6 = 156).
// Display height is 128, so there are 16 rows (16x8 = 128).
// Total number of characters in landscape mode = 26x16 = 416.
void ST7735s::gotoXY(uint8_t x, uint8_t y) { // position cursor on character x,y grid, where 0<x<20, 0<y<19.
	curX = x;
	curY = y;
}
void ST7735s::gotoLine(uint8_t y) { // position character cursor to start of line y, where 0<y<19.
	curX = 0;
	curY = y;
}

void ST7735s::advanceCursor() { // moves character cursor to next position, assuming portrait orientation
	curX++;
	if (curX > 20) {
		curY++;
		curX = 1;
	}
	if (curY>19)
		curY = 1;
}

void ST7735s::setOrientation(int degrees) {
	uint8_t arg;
	switch (degrees) {
		case 90: arg = 0x60; break;
		case 180: arg = 0xC0; break;
		case 270: arg = 0xA0; break;
		default: arg = 0x00;
	}
	writeCmd(MADCTL);
	writeByte(arg);
}

void ST7735s::putCh(char ch, uint8_t x, uint8_t y, int color) { // write ch to display X,Y coordinates using ASCII 5x7 font
	int pixel;
	uint8_t row, col, bit, data, mask = 0x01;
	setAddrWindow(x, y, x+4, y+6);
	writeCmd(RAMWR);
	for (row=0; row<7;row++) {
		for (col=0; col<5;col++) {
			data = FONT_CHARS[ch-32][col];
			bit = data & mask;
			if (bit == 0) pixel = BLACK;
			else pixel = color;
			writeWord(pixel);
		}
		mask <<= 1;
	}
}

void ST7735s::writeChar(char ch, int color) {
	putCh(ch, curX*6, curY*8, color);
	advanceCursor();
}

void ST7735s::writeString(char *text, int color) {
	for (; *text; text++)
	writeChar(*text, color);
}

void ST7735s::writeInt(int i) {
	char str[8];
	itoa(i, str, 10);
	writeString(str, WHITE);
}

void ST7735s::writeHex(int i) {
	char str[8];
	itoa(i, str, 16);
	writeString(str, WHITE);
}
