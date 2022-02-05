#include "Window.h"

Window::Window() :
    _hInst(GetModuleHandle(NULL)),
    _hWnd(NULL),
    _plt(nullptr)
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
        return pWin->processMessage(hWnd, message, wParam, lParam);

    return 0;
}

LRESULT CALLBACK Window::processMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND: onCommand(LOWORD(wParam));                           break;
    case WM_DESTROY: PostQuitMessage(0);                                  break;
    default:         return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return FALSE;
}

void Window::onCommand(WORD id) {
    switch (id) {
    case IDC_CLOSE: PostQuitMessage(0); break;
    default:                            break;
    }
}

void Window::createGUI() {
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, _T("Segoe UI"));

    _plt = new Plotter(_T("Plot"), { 14, 14 }, { WNDSIZE.cx - 14 * 2, 500 }, _hWnd, _hInst);
    _plt->redraw();

    CreateWindow(WC_BUTTON, _T("Закрыть"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, WNDSIZE.cx - 100 - 14, WNDSIZE.cy - 25 - 14, 100, 25, _hWnd, (HMENU)IDC_CLOSE, _hInst, NULL);
    SendMessage(GetDlgItem(_hWnd, IDC_CLOSE), WM_SETFONT, WPARAM(hFont), true);
    //createButton(_T("< Убрать"), getRemoveButtonPos(boxSize), { BUTTON_WIDTH, BUTTON_HEIGHT }, (HMENU)ID_BUTTON_REMOVE);
    //createButton(_T("Выгрузить"), getUploadButtonPos(clientSize), { BUTTON_WIDTH, BUTTON_HEIGHT }, (HMENU)ID_BUTTON_UPLOAD);
    //createCheckBox(_T("Добавлять файлы любой вложенности"), { INDENT, clientSize.y - INDENT - BUTTON_HEIGHT }, { BUTTON_WIDTH * 3, BUTTON_HEIGHT }, (HMENU)ID_CHECKBOX_ADDING_MODE);
}


HINSTANCE Window::get_hInst() {
    return _hInst;
}

HWND Window::get_hWnd() {
    return _hWnd;
}