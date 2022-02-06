#include "Window.h"

int main() {
    Window* wnd = new Window();
    //Plotter* plt = new Plotter(_T("Plot"), { 0, 0 }, { 800, 600 });

    //plt->addGraph({ {{-1,3},{0,0},{0,2},{2,5},{5,6}},RGB(255,0,255), 1 });
    //plt->addGraph({ {{0,3},{2,7},{3,-4},{3,-2},{22,-3}},RGB(255,0,0), 1 });
    //
    //plt->redraw();

    MSG message;

    //Sorts<EvilInt> s = {};
    //s.generateArray(50);
    //s.quickSort(0, 49);
    //s.printArray();

    while (GetMessage(&message, NULL, NULL, NULL)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return EXIT_SUCCESS;
}