#include "Window.h"

int main() {
    Window* wnd = new Window();

    MSG message;

    while (GetMessage(&message, NULL, NULL, NULL)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

	delete wnd;

    return EXIT_SUCCESS;
}