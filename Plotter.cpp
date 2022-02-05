#include "Plotter.h"

Plotter::Plotter(LPCTSTR name, const POINT& pos, const SIZE& size, HWND hPWnd /*= NULL*/, HINSTANCE hInst /*= GetModuleHandle(NULL)*/) :
    _size(size),
    _hInst(hInst),
    _hWnd(NULL),
    _painter(nullptr),
    _graphics({}),
    _step(INITIALSTEP),
    _divisionPrice(1),
    _offset({ (long) round(size.cx / 2), (long) round(size.cy / 2) })
{
    assert(registerClass(name));

    DWORD style = (hPWnd != NULL)? WS_VISIBLE | WS_CHILD | WS_BORDER : WS_VISIBLE | WS_CAPTION | WS_SYSMENU;

    _hWnd = CreateWindow(
        name,
        _T(""),
        style,
        pos.x,
        pos.y,
        (hPWnd != NULL)? size.cx : size.cx + 14,
        (hPWnd != NULL)? size.cy : size.cy + 39,
        hPWnd,
        NULL,
        _hInst,
        this);

    _painter = new Painter(GetDC(_hWnd), size);

    clearField();
}

Plotter::~Plotter() {
    delete _painter;

    ReleaseDC(_hWnd, _painter->getDC(0));
    ReleaseDC(_hWnd, _painter->getDC(1));

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
    switch (message) {
    case WM_MOUSEMOVE:  onMouseMove({ LOWORD(lParam), HIWORD(lParam) });     break;
    case WM_MOUSEWHEEL: onScroll(GET_WHEEL_DELTA_WPARAM(wParam));            break;
    case WM_DESTROY:    PostQuitMessage(WM_QUIT);                            break;
    default:            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return FALSE;
}

void Plotter::onMouseMove(POINT pos) {
    static POINT oldMousePos = { -1, -1 };
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        if (pos.x > 65000)
            return;

        if (oldMousePos.x == -1)
            oldMousePos = pos;
        else {
            _offset = { _offset.x + pos.x - oldMousePos.x, _offset.y - pos.y + oldMousePos.y };

            oldMousePos = pos;
        }
    }
    else {
        oldMousePos = { -1, -1 };
    }

    redraw();
}

void Plotter::onScroll(short delta) {
    assert(delta != 0);
    int deltaStep = 2 * (delta / abs(delta));

    if (deltaStep < 0 && _divisionPrice >= pow(4, 7))
        return;

    POINT mousePos = {};
    getMousePos(&mousePos);

    float k = (float) (_step + deltaStep) / _step;

    _offset = {
        mousePos.x + (LONG) ((_offset.x - mousePos.x) * k),
        _size.cy - mousePos.y + (LONG) ((_offset.y - _size.cy + mousePos.y) * k)
    };

    _step += deltaStep;

    if (_step < INITIALSTEP / 4) {
        _step = INITIALSTEP;

        _divisionPrice *= 4;
    }

    if (_divisionPrice >= 4 && _step > INITIALSTEP) {
        _step = INITIALSTEP / 4;

        _divisionPrice /= 4;
    }

    redraw();
}

void Plotter::plotGraphs() {
    for (int i = 0; i < _graphics.size(); ++i)
        plotByPoint(i);
}

void Plotter::plotByPoint(int i) {
    _painter->setColor    (_graphics[i].color, 2);
    _painter->setFillColor(_graphics[i].color);

    for (int point = 0; point < _graphics[i].points.size(); ++point)
        drawPoint(_graphics[i].points[point], 4);

    if (_graphics[i].connectPoints) {
        for (int point = 1; point < _graphics[i].points.size(); ++point) {
            _painter->drawLine(
                { _graphics[i].points[point - 1].x * _step / _divisionPrice + _offset.x, _graphics[i].points[point - 1].y * _step / _divisionPrice + _offset.y },
                { _graphics[i].points[point    ].x * _step / _divisionPrice + _offset.x, _graphics[i].points[point    ].y * _step / _divisionPrice + _offset.y }
            );
        }
    }
}

void Plotter::drawCursorCoords() {
    POINT pos = {};
    getMousePos(&pos);

    HFONT font = CreateFont(15, 0, 0, 0, FW_MEDIUM, ANSI_CHARSET, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("Segoe UI"));
    HGDIOBJ old = SelectObject(_painter->getDC(), font);

    std::string coords = "(" + std::to_string((LONG)round((float)(pos.x - _offset.x) / _step * _divisionPrice)) + "; " + std::to_string((LONG)round((float)(_size.cy - pos.y - _offset.y) / _step * _divisionPrice)) + ")";
    RECT rc = { pos.x + 10, pos.y + 10, _offset.x - 10, pos.y + 20 };

    DrawTextA(_painter->getDC(), coords.c_str(), coords.length(), &rc, DT_LEFT | DT_NOCLIP);

    SelectObject(_painter->getDC(), old);
    DeleteObject(font);
}

void Plotter::redraw() {
    _painter->beginPaint();

    clearField();

    plotGraphs();

    _painter->endPaint();
}

void Plotter::clearField() {
    _painter->setFillColor(BACKGROUNDCOLOR);

    Rectangle(_painter->getDC(), -1, -1, _size.cx + 1, _size.cy + 1);

    drawField();

    drawCursorCoords();
}

void Plotter::drawField() {
    drawGrid();

    drawAxes();
}

void Plotter::drawGrid() {
    _painter->setColor(RGB(200, 200, 200));

    for (int y = _offset.y % _step - _step; y <= _size.cy; y += _step) {
        _painter->drawLine({ 0, y }, { _size.cx, y });
    }

    for (int x = _offset.x % _step; x <= _size.cx; x += _step) {
        _painter->drawLine({ x, 0 }, { x, _size.cy });
    }
}

void Plotter::drawAxes() {
    _painter->setColor(RGB(0, 0, 0));

    _painter->drawLine({ _offset.x, 0 }, { _offset.x, _size.cy });
    _painter->drawLine({ 0, _offset.y }, { _size.cx, _offset.y });

    signAxes();
}

void Plotter::signAxes() {
    HFONT font = CreateFont(13, 0, 0, 0, 0, ANSI_CHARSET, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("Segoe UI"));
    HGDIOBJ old = SelectObject(_painter->getDC(), font);

    for (int y = _offset.y % (2 * _step) - 2 * _step; y <= _size.cy; y += _step * 2) {
        _painter->drawLine({ _offset.x - 5, y }, { _offset.x + 5, y });
        _painter->drawLine({ _offset.x - 3, y + _step }, { _offset.x + 3, y + _step });

        if (y == _offset.y) 
            continue;

        std::string num = std::to_string((y - _offset.y) / _step * _divisionPrice);
        RECT rc = { _offset.x - num.length() * 8 - 10, _size.cy - y - 5, _offset.x - 10, _size.cy - y + 20 };

        DrawTextA(_painter->getDC(), num.c_str(), num.length(), &rc, DT_RIGHT | DT_EDITCONTROL | DT_WORDBREAK);
    }

    for (int x = _offset.x % (2 * _step) - 2 * _step; x <= _size.cx; x += _step * 2) {
        _painter->drawLine({ x, _offset.y - 5 }, { x, _offset.y + 5 });
        _painter->drawLine({ x + _step, _offset.y - 3 }, { x + _step, _offset.y + 3 });

        std::string num = std::to_string((x - _offset.x) / _step * _divisionPrice);
        RECT rc = { x - _step * 2, _size.cy - _offset.y + 5, x + _step * 2, _size.cy - _offset.y + 5 + 25};

        DrawTextA(_painter->getDC(), num.c_str(), num.length(), &rc, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
    }


    SelectObject(_painter->getDC(), old);
    DeleteObject(font);
}

void Plotter::drawPoint(const POINT& point, LONG r) {
    POINT center = {
        point.x * _step / _divisionPrice + _offset.x,
        point.y * _step / _divisionPrice + _offset.y
    };

    _painter->drawCircle(center, r);
}

void Plotter::addGraph(const Graph& graph) {
    _graphics.push_back(graph);
}

void Plotter::getMousePos(POINT* pos) {
    GetCursorPos(pos);
    ScreenToClient(_hWnd, pos);
}