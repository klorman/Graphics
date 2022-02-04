#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <cassert>
#include <tchar.h>
#include <iostream>

class Plotter {
private:
	SIZE _size;
	HINSTANCE _hInst;
	HWND _hWnd;
	HDC _canvas[2];
	int _canvasUpdateLock;
	HBITMAP _bmp,
            _stockBmp;

	int _step;
	POINT _offset;

	static LRESULT CALLBACK PlotterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	       LRESULT CALLBACK OnMessage  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void onMouseMove(POINT pos);

	bool registerClass(LPCTSTR className);

	void drawField();
	void drawGrid();
	void drawAxes();
	void drawLine(POINT start, POINT end);
	void setColor(COLORREF color);

public:
	Plotter(LPCTSTR name, POINT pos, SIZE size, HWND hPWnd = NULL, HINSTANCE hInst = GetModuleHandle(NULL));
	~Plotter();

	void clearField();
	void beginPaint();
	void endPaint();

	HDC& getDC();
};