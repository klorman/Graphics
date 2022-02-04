#include "Plotter.h"

Plotter::Plotter(LPCTSTR name, POINT pos, SIZE size, HWND hPWnd /*= NULL*/, HINSTANCE hInst /*= GetModuleHandle(NULL)*/) :
    _size(size),
    _hInst(hInst),
    _hWnd(NULL),
    _canvas(),
    _canvasUpdateLock(0),
    _bmp(NULL),
    _stockBmp(NULL),
    _step(INITIALSTEP),
    _divisionPrice(1),
    _offset({ (long) round(size.cx / 2), (long) round(size.cy / 2) })
{
    assert(registerClass(name));

    DWORD style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
    if (hPWnd != NULL) {
        style |= WS_CHILD;
    }

    _hWnd = CreateWindow(
        name,
        _T(""),
        style,
        pos.x,
        pos.y,
        size.cx + 14,
        size.cy + 39,
        NULL,
        NULL,
        _hInst,
        this);

    _canvas[0] = GetDC(_hWnd);
    _canvas[1] = CreateCompatibleDC(_canvas[0]);

    _bmp = CreateCompatibleBitmap(_canvas[0], _size.cx, _size.cy);

    clearField();
}

Plotter::~Plotter() {
    DeleteObject(_bmp);

    ReleaseDC(_hWnd, _canvas[0]);
    ReleaseDC(_hWnd, _canvas[1]);

    DestroyWindow(_hWnd);
}

bool Plotter::registerClass(LPCTSTR className) {
    WNDCLASS windowClass = {};
    windowClass.style = NULL;
    windowClass.lpfnWndProc = PlotterProc;
    windowClass.cbClsExtra = NULL;
    windowClass.cbWndExtra = NULL;
    windowClass.hInstance = _hInst;
    windowClass.hIcon = NULL;
    windowClass.hCursor = LoadCursor(0, IDC_CROSS);
    windowClass.hbrBackground = (HBRUSH) COLOR_APPWORKSPACE;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = className;

    return RegisterClass(&windowClass) != 0;
}

LRESULT CALLBACK Plotter::PlotterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LONG dwNewLong;
    Plotter* pWnd;

    if (message == WM_NCCREATE) {
        dwNewLong = (long)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLong(hWnd, GWL_USERDATA, dwNewLong);
        return TRUE;
    }
    else {
        pWnd = (Plotter*)GetWindowLong(hWnd, GWL_USERDATA);
    }

    if (pWnd) return pWnd->OnMessage(hWnd, message, wParam, lParam);

    return FALSE;
}

LRESULT CALLBACK Plotter::OnMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        default:
            break;
        }
        break;

    case WM_MOUSEMOVE:
        onMouseMove({ LOWORD(lParam), HIWORD(lParam) });
        break;

    case WM_MOUSEWHEEL:
        onScroll(GET_WHEEL_DELTA_WPARAM(wParam));
        break;

    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return FALSE;
}

void Plotter::onMouseMove(POINT pos) {
    static POINT oldMousePos = { -1, -1 };

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        if (oldMousePos.x == -1)
            oldMousePos = pos;
        else {
            _offset = { _offset.x + pos.x - oldMousePos.x, _offset.y - pos.y + oldMousePos.y };

            oldMousePos = pos;

            clearField();
        }
    }
    else {
        oldMousePos = { -1, -1 };
    }
}

void Plotter::onScroll(short delta) {
    POINT mousePos = {};
    GetCursorPos(&mousePos);
    ScreenToClient(_hWnd, &mousePos);
    mousePos = {
        mousePos.x,
        _size.cy - mousePos.y
    };

    assert(delta != 0);
    int deltaStep = 2 * (delta / abs(delta)); //(short) (delta / 60); // = 2
    float k = (float) (_step + deltaStep) / _step;

    _offset = {
        mousePos.x + (LONG) ((_offset.x - mousePos.x) * k),
        mousePos.y + (LONG) ((_offset.y - mousePos.y) * k)
    };

    _step += deltaStep;

    if (_step < INITIALSTEP / 4) {
        _step = INITIALSTEP;

        _divisionPrice *= 4;
    }

    if (_step > INITIALSTEP) {
        _step = INITIALSTEP / 4;

        _divisionPrice /= 4;
    }

    clearField();
}

void Plotter::clearField() {
    beginPaint();

    Rectangle(_canvas[_canvasUpdateLock], -1, -1, _size.cx + 1, _size.cy + 1);

    drawField();

    endPaint();
}

void Plotter::drawField() {
    drawGrid();

    drawAxes();
}

void Plotter::drawGrid() {
    setColor(RGB(200, 200, 200));

    for (int y = _offset.y % _step - _step; y <= _size.cy; y += _step) {
        drawLine({ 0, y }, { _size.cx, y });
    }

    for (int x = _offset.x % _step; x <= _size.cx; x += _step) {
        drawLine({ x, 0 }, { x, _size.cy });
    }
}

void Plotter::drawAxes() {
    setColor(RGB(0, 0, 0));

    drawLine({ _offset.x, 0 }, { _offset.x, _size.cy });
    drawLine({ 0, _offset.y }, { _size.cx, _offset.y });
}

void Plotter::drawLine(POINT start, POINT end) {
    MoveToEx(_canvas[_canvasUpdateLock], start.x, _size.cy - start.y, NULL);
    LineTo(_canvas[_canvasUpdateLock], end.x, _size.cy - end.y);
    UpdateWindow(_hWnd);
}

void Plotter::setColor(COLORREF color) {
    HGDIOBJ obj = CreatePen(PS_SOLID, 1, color);

    obj = SelectPen(_canvas[_canvasUpdateLock], obj);

    DeleteObject(obj);
}

void Plotter::beginPaint() {
    _canvasUpdateLock = 1;

    _stockBmp = (HBITMAP)SelectObject(_canvas[1], _bmp);
}

void Plotter::endPaint() {
    _canvasUpdateLock = 0;

    BitBlt(_canvas[0], 0, 0, _size.cx, _size.cy, _canvas[1], 0, 0, SRCCOPY);

    SelectObject(_canvas[1], _stockBmp);
}

HDC& Plotter::getDC() {
    return _canvas[0];
}