#include "Window.h"

Window::Window() :
    _hInst (GetModuleHandle(NULL)),
    _hWnd  (NULL),
    _plt   (nullptr),
    _sorts (new Sorts<EvilInt>)
{
    assert(registerClass());

    POINT screenSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) },
          wndPos = { (screenSize.x - WNDSIZE.cx) >> 1, (screenSize.y - WNDSIZE.cy) >> 1 };

    _hWnd = CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        _T("SortingGraphs"),
        _T("Зависимость количества сравнений от количества присваиваний"),
        WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
        wndPos.x,
        wndPos.y,
        WNDSIZE.cx + 14,
        WNDSIZE.cy + 39,
        NULL,
        NULL,
        _hInst,
        this);

    createGUI();

    SetErrorMode(SEM_FAILCRITICALERRORS);
}

Window::~Window() {
    delete _plt;
    delete _sorts;

    DestroyWindow(_hWnd);

    UnregisterClass(TEXT("SortingGraphs"), _hInst);
}

bool Window::registerClass() {
    WNDCLASS windowClass = {};
    windowClass.style = NULL;
    windowClass.lpfnWndProc = &wndProc;
    windowClass.cbClsExtra = NULL;
    windowClass.cbWndExtra = NULL;
    windowClass.hInstance = _hInst;
    windowClass.hIcon = NULL;
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = _T("SortingGraphs");

    return RegisterClass(&windowClass) != 0;
}

LRESULT CALLBACK Window::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LONG dwNewLong;
    Window* pWin;

    if (message == WM_NCCREATE) {
        dwNewLong = (long)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLong(hWnd, GWL_USERDATA, dwNewLong);
        return TRUE;
    }
    else
        pWin = (Window*)GetWindowLong(hWnd, GWL_USERDATA);

    if (pWin)
        return pWin->onMessage(hWnd, message, wParam, lParam);

    return 0;
}

LRESULT CALLBACK Window::onMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND  : onCommand(LOWORD(wParam));                           break;
    case WM_DESTROY  : PostQuitMessage(0);                                  break;
    default          : return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return FALSE;
}

void Window::onCommand(WORD id) {
    switch (id) {
    case IDC_CLOSE             : PostQuitMessage(0);                                                                                              break;
    case IDC_CLEAR             : for (int i = 0; i < _plt->getNumberOfGraphs(); ++i) _plt->editGraphPoints((GRAPHICS) i, {}); _plt->clearField(); break;
    case IDC_RECALCSELECTED    : recalcSelected();      _plt->redraw();                                                                           break;
    case IDC_RECALCBUBBLE      : recalcSort(BUBBLE);    _plt->redraw();                                                                           break;
    case IDC_RECALCSELECTION   : recalcSort(SELECTION); _plt->redraw();                                                                           break;
    case IDC_RECALCQUICK       : recalcSort(QUICK);     _plt->redraw();                                                                           break;
    case IDC_RECALCRADIX       : recalcSort(RADIX);     _plt->redraw();                                                                           break;
    case IDC_RECALCBOGO        : recalcSort(BOGO);      _plt->redraw();                                                                           break;
    default                    :                                                                                                                  break;
    }
}

void Window::createGUI() {
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, _T("Segoe UI"));

    _plt = new Plotter(_T("Plot"), { 14, 14 }, { WNDSIZE.cx - 14 * 2, 500 }, _hWnd, _hInst);
    _plt->redraw();

    _plt->addGraph({ {}, RGB(255, 0, 0),   true });
    _plt->addGraph({ {}, RGB(255, 0, 255), true });
    _plt->addGraph({ {}, RGB(0, 0, 255),   true });
    _plt->addGraph({ {}, RGB(0, 255, 0),   true });
    _plt->addGraph({ {}, RGB(0, 255, 255), true });

    CreateWindow(WC_BUTTON, _T("Закрыть"),               WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx - 100 - 14, WNDSIZE.cy - 28 - 14, 100, 28, _hWnd, (HMENU)IDC_CLOSE,          _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Пересчитать выбранные"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx - 300 - 28, WNDSIZE.cy - 28 - 14, 200, 28, _hWnd, (HMENU)IDC_RECALCSELECTED, _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Очистить"),              WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx - 400 - 42, WNDSIZE.cy - 28 - 14, 100, 28, _hWnd, (HMENU)IDC_CLEAR,          _hInst, NULL);

    CreateWindow(WC_BUTTON, _T("Пересчитать"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx / 2 - 14 - 100, 500 + 28,          100, 28, _hWnd, (HMENU)IDC_RECALCBUBBLE,    _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Пересчитать"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx / 2 - 14 - 100, 500 + 42 + 14 * 2, 100, 28, _hWnd, (HMENU)IDC_RECALCSELECTION, _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Пересчитать"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx / 2 + 14,       500 + 28,          100, 28, _hWnd, (HMENU)IDC_RECALCQUICK,     _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Пересчитать"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx / 2 + 14,       500 + 42 + 14 * 2, 100, 28, _hWnd, (HMENU)IDC_RECALCRADIX,     _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Пересчитать"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WNDSIZE.cx / 2 - 14 - 100, 500 + 56 + 14 * 4, 100, 28, _hWnd, (HMENU)IDC_RECALCBOGO,      _hInst, NULL);

    CreateWindow(WC_BUTTON, _T("Bubble sort"),    WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, 14,                   500 + 28         , (WNDSIZE.cx - 200 - 14 * 6) / 2, 28, _hWnd, (HMENU)IDC_CHECKBUBBLESORT, _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Selection sort"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, 14,                   500 + 42 + 14 * 2, (WNDSIZE.cx - 200 - 14 * 6) / 2, 28, _hWnd, (HMENU)IDC_CHECKSELECTIONSORT, _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Quick sort"),     WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_RIGHT   , WNDSIZE.cx / 2 + 128, 500 + 28         , (WNDSIZE.cx - 200 - 14 * 6) / 2, 28, _hWnd, (HMENU)IDC_CHECKQUICKSORT, _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Radix sort"),     WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_RIGHT   , WNDSIZE.cx / 2 + 128, 500 + 42 + 14 * 2, (WNDSIZE.cx - 200 - 14 * 6) / 2, 28, _hWnd, (HMENU)IDC_CHECKRADIXSORT, _hInst, NULL);
    CreateWindow(WC_BUTTON, _T("Bogo sort"),      WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, 14,                   500 + 56 + 14 * 4, (WNDSIZE.cx - 200 - 14 * 6) / 2, 28, _hWnd, (HMENU)IDC_CHECKBOGOSORT, _hInst, NULL);

    for (int ID = IDC_CLOSE; ID <= IDC_CHECKBOGOSORT; ++ID)
        SendMessage(GetDlgItem(_hWnd, ID), WM_SETFONT, WPARAM(hFont), true);
}

void Window::recalcSelected() {
    for (int ID = IDC_CHECKBUBBLESORT; ID <= IDC_CHECKBOGOSORT; ++ID)
        if (SendDlgItemMessage(_hWnd, ID, BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
            switch (ID) {
            case IDC_CHECKBUBBLESORT     : recalcSort(BUBBLE);    break;
            case IDC_CHECKSELECTIONSORT  : recalcSort(SELECTION); break;
            case IDC_CHECKQUICKSORT      : recalcSort(QUICK);     break;
            case IDC_CHECKRADIXSORT      : recalcSort(RADIX);     break;
            case IDC_CHECKBOGOSORT       : recalcSort(BOGO);      break;
            }
        }
}

void Window::recalcSort(GRAPHICS id) {
    std::vector<POINT> points = {};
    int numberOfComparisons = 0;
    
    for (int i = 5; numberOfComparisons < 100000; i += 1) {
        _sorts->generateArray(i);
		_sorts->startSort(id);

		numberOfComparisons = _sorts->getNumberOfComparisons();

		if (numberOfComparisons < 100000)
			points.push_back({ _sorts->getNumberOfAssignments(), numberOfComparisons });
    }

    _plt->editGraphPoints(id, points);
}

HINSTANCE Window::get_hInst() {
    return _hInst;
}

HWND Window::get_hWnd() {
    return _hWnd;
}