#pragma once

#include "Plotter.h"
#include "Sorts.h"

#include <CommCtrl.h>
#pragma comment(lib,"Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define IDC_CLOSE               1001
#define IDC_RECALCSELECTED      1002
#define IDC_CLEAR               1003
#define IDC_RECALCBUBBLE        1004
#define IDC_RECALCSELECTION     1005
#define IDC_RECALCQUICK         1006
#define IDC_RECALCRADIX         1007
#define IDC_RECALCBOGO          1008
#define IDC_CHECKBUBBLESORT     1009
#define IDC_CHECKSELECTIONSORT  1010
#define IDC_CHECKQUICKSORT      1011
#define IDC_CHECKRADIXSORT      1012
#define IDC_CHECKBOGOSORT       1013

class Window {
private:
	HINSTANCE _hInst;

	HWND _hWnd;
	Plotter* _plt;
	Sorts<EvilInt>* _sorts;

	const SIZE WNDSIZE = { 800, 800 };

	bool registerClass();

	static LRESULT CALLBACK wndProc  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	       LRESULT CALLBACK onMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void onCommand(WORD id);

	void recalcSelected();
	void recalcBubble();
	void recalcSelection();
	void recalcQuick();
	void recalcRadix();
	void recalcBogo();

	void createGUI();

public:
	Window();
	~Window();

	HINSTANCE get_hInst();
	HWND      get_hWnd();
};