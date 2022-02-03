#include "Plotter.h"

int main() {
    Plotter* plt = new Plotter(_T("Plot"), { 0, 0 }, { 800, 600 });

    MSG message;

    while (GetMessage(&message, NULL, NULL, NULL)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return EXIT_SUCCESS;
}