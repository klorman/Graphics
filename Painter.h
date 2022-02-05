#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <cassert>

class Painter {
private:
	HDC _canvasBuf[2];
	int _canvasUpdateLock;
	HBITMAP _bmp,
	        _stockBmp;
	SIZE _size;

public:
	Painter(HDC canvas, const SIZE& size);
	~Painter();

	void beginPaint();
	void endPaint();

	void drawLine(const POINT& start, const POINT& end);
	void drawCircle(const POINT& center, LONG r);
	void setColor(COLORREF color, int thickness = 1, int style = PS_SOLID);
	void setFillColor(COLORREF color);

	HDC& getDC(int i = -1);
};

