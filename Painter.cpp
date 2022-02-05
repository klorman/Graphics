#include "Painter.h"

Painter::Painter(HDC canvas, const SIZE& size) :
	_canvasBuf(),
	_canvasUpdateLock(0),
	_bmp(NULL),
	_stockBmp(NULL),
	_size(size)
{
	_canvasBuf[0] = canvas;
	_canvasBuf[1] = CreateCompatibleDC(canvas);
	_bmp = CreateCompatibleBitmap(canvas, size.cx, size.cy);
}

Painter::~Painter() {
	DeleteObject(_bmp);
}

void Painter::drawCircle(const POINT& center, LONG r) {
    Ellipse(getDC(), center.x - r, _size.cy - center.y - r, center.x + r, _size.cy - center.y + r);
}

void Painter::drawLine(const POINT& start, const POINT& end) {
    MoveToEx(getDC(), start.x, _size.cy - start.y, NULL);
    LineTo(getDC(), end.x, _size.cy - end.y);
}

void Painter::setColor(COLORREF color, int thickness /*= 1*/, int style /*= PS_SOLID*/) {
    HPEN pen = CreatePen(style, thickness, color);

    pen = SelectPen(getDC(), pen);

    DeletePen(pen);
}

void Painter::setFillColor(COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);

    brush = SelectBrush(getDC(), brush);

    DeleteObject(brush);
}

void Painter::beginPaint() {
    _canvasUpdateLock = 1;

    _stockBmp = (HBITMAP)SelectObject(_canvasBuf[1], _bmp);
}

void Painter::endPaint() {
    _canvasUpdateLock = 0;

    BitBlt(_canvasBuf[0], 0, 0, _size.cx, _size.cy, _canvasBuf[1], 0, 0, SRCCOPY);

    SelectObject(_canvasBuf[1], _stockBmp);
}

HDC& Painter::getDC(int i /* = -1*/) {
    assert(abs(i) <= 1);

    return (i == -1) ? _canvasBuf[_canvasUpdateLock] : _canvasBuf[i];
}