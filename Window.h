#pragma once

#include "Plotter.h"

#include <CommCtrl.h>
#pragma comment(lib,"Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define IDC_CLOSE 1001

class Window {
private:
	HINSTANCE _hInst;
	HWND _hWnd;
	Plotter* _plt;

	const SIZE WNDSIZE = { 800, 800 };

	bool registerClass();

	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK processMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void onCommand(WORD id);

	void createGUI();

public:
	Window();
	~Window();

	HINSTANCE get_hInst();
	HWND      get_hWnd();
};