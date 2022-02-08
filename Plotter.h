#pragma once

#include <Windows.h>
#include <cassert>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <string>
#include "Painter.h"

enum GRAPHICS {
	BUBBLE, SELECTION, QUICK, RADIX, BOGO
};

struct Graph {
	std::vector<POINT> points;
	COLORREF           color;
	BOOL               connectPoints;
};

class Plotter {
private:
	SIZE               _size;
	HINSTANCE          _hInst;
	HWND               _hWnd;
	Painter*           _painter;
	std::vector<Graph> _graphics;

	const int      INITIALSTEP     = 40;
	const COLORREF BACKGROUNDCOLOR = RGB(255, 255, 255);

	int   _step;
	int   _divisionPrice;
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
	void drawPoint(const POINT& point, LONG r);

public:
	Plotter(LPCTSTR name, const POINT& pos, const SIZE& size, HWND hPWnd = NULL, HINSTANCE hInst = GetModuleHandle(NULL));
	~Plotter();

	void redraw();
	void clearField();

	void plotGraphs();
	void plotByPoint(int i);
	void addGraph(const Graph& graph);
	void editGraphPoints(GRAPHICS name, const std::vector<POINT>& points);
	int  getNumberOfGraphs();
	
	void getMousePos(POINT* pos);
	bool isMouseOnWnd();
};