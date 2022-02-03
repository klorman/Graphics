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
	HDC _canvas;

	int _step;
	POINT _offset;

	bool registerClass(LPCTSTR className);
	void drawField();
	void drawGrid();
	void drawAxes();
	void drawLine(POINT start, POINT end);
	void setColor(COLORREF color);

public:
	void clearField();

	Plotter(LPCTSTR name, POINT pos, SIZE size, HWND hPWnd = NULL, HINSTANCE hInst = GetModuleHandle(NULL));
	~Plotter();

	HDC& getDC();
};

LRESULT CALLBACK PlotterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void onPaint(HWND hWnd);