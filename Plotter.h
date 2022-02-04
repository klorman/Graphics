#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <cassert>
#include <tchar.h>
#include <iostream>
#include <string>

class Plotter {
private:
	SIZE _size;
	HINSTANCE _hInst;
	HWND _hWnd;
	HDC _canvas[2];
	int _canvasUpdateLock;
	HBITMAP _bmp,
            _stockBmp;

	const int INITIALSTEP = 40;

	int _step;
	int _divisionPrice;
	POINT _offset;

	static LRESULT CALLBACK PlotterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	       LRESULT CALLBACK OnMessage  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void onMouseMove(POINT pos);
	void onScroll(short delta);

	bool registerClass(LPCTSTR className);

	void drawCursorCoords();
	void drawField();
	void drawGrid();
	void drawAxes();
	void signAxes();
	void drawLine(POINT start, POINT end);
	void setColor(COLORREF color);

public:
	Plotter(LPCTSTR name, POINT pos, SIZE size, HWND hPWnd = NULL, HINSTANCE hInst = GetModuleHandle(NULL));
	~Plotter();

	void redraw();
	void clearField();
	void beginPaint();
	void endPaint();

	HDC& getDC();
	void getMousePos(POINT& pos);
};