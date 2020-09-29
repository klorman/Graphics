#include "TXLib.h"

#include <iostream> 
#include <algorithm>
#include <ctime>
#include <fstream>
#include <string.h>

class Window {
private:
	int x0_, y0_, x1_, y1_;
	const char* text_;

	void grid();
	void axes();
public:
	void draw_numbers(const char* number, int x);
	void draw_field();
	void draw_text();

	Window(int x0, int y0, int x1, int y1, const char* text) {
		x0_ = x0;
		y0_ = y0;
		x1_ = x1;
		y1_ = y1;
		text_ = text;
	}
};

class CoordSys {
private:
	int x0_, y0_, x1_, y1_;
	double scaleX_, scaleY_;

	double find_x(double x);
	double find_y(double y);

public:
	int draw_point(int x, int y);
	void redraw(int* numbers_of_swaps);

	CoordSys(int x0, int y0, int x1, int y1, double scaleX, double scaleY) {
		x0_ = x0;
		y0_ = y0;
		x1_ = x1;
		y1_ = y1;
		scaleX_ = scaleX;
		scaleY_ = scaleY;
	}
};

class Button {
	int x0_, y0_, x1_, y1_;
	const char* text_;

public:
	void draw_button();
	int is_button_pressed();

	Button(int x0, int y0, int x1, int y1, const char* text) {
		x0_ = x0;
		y0_ = y0;
		x1_ = x1;
		y1_ = y1;
		text_ = text;
	}
};

void CoordSys::redraw(int* numbers_of_swaps) {
	int end = 0;

	for (int size_of_array = 10; size_of_array < 3000; size_of_array += 10) {
		if (!end) if (draw_point(size_of_array, numbers_of_swaps[size_of_array / 10])) end = 1;
	}
}

int CoordSys::draw_point(int x0, int y0) {
	double x = x0 * scaleX_, y = y0 * scaleY_;

	if (x < (x1_ - x0_ - 12) && y < (y1_ - y0_ - 12)) {
		txCircle(find_x(x), find_y(y), 2);
		return 0;
	}

	return 1;
}

double CoordSys::find_x(double x) {
	return x0_ + x + 10;
}

double CoordSys::find_y(double y) {
	return y1_ - y - 10;
}

void Window::draw_numbers(const char* number, int x) {
	txSetColor(TX_BLACK);
	txDrawText(10 + x, 430, 30 + x, 460, number);
}

void Window::draw_text() {
	txSetColor(TX_WHITE);
	txRectangle(x0_ + 50, y1_ + 10, x1_, y1_ + 40);

	txSetColor(TX_BLACK);
	txDrawText(x0_, y1_ + 10, x1_, y1_ + 40, text_);
}

void Window::grid() {
	txSetColor(TX_LIGHTGRAY);

	for (int x = x0_; x < x1_; x += 10) txLine(x, y0_, x, y1_);
	for (int y = y0_; y < y1_; y += 10) txLine(x0_, y, x1_, y);
}

void Window::axes() {
	txSetColor(TX_BLACK);

	txLine(x0_, y1_ - 10, x1_, y1_ - 10);
	txLine(x0_ + 10, y0_, x0_ + 10, y1_);

	txLine(x1_, y1_ - 10, x1_ - 5, y1_ - 10 - 5);
	txLine(x1_, y1_ - 10, x1_ - 5, y1_ - 10 + 5);

	txLine(x0_ + 10, y0_, x0_ + 10 + 5, y0_ + 5);
	txLine(x0_ + 10, y0_, x0_ + 10 - 5, y0_ + 5);

	for (int x = x0_; x < x1_; x += 10) txLine(x, y1_ - 10 - 1, x, y1_ - 10 + 2);
	for (int y = y0_; y < y1_; y += 10) txLine(x0_ + 10 - 1, y, x0_ + 10 + 2, y);
}

void Window::draw_field() {
	txRectangle(x0_, y0_, x1_, y1_);
	draw_text();
	grid();
	axes();
	//char result[17];
	//for (int i = 10; i < 3000; i += 20) {
	//	_itoa_s(i, result, 10);
	//	draw_numbers(result, i);
	//}

}

void Button::draw_button() {
	txRectangle(x0_, y0_, x1_, y1_);
	txDrawText(x0_, y0_, x1_, y1_, text_);
}

int Button::is_button_pressed() {
	double x = txMouseX(), y = txMouseY();

	if (x >= x0_ && x <= x1_ && y >= y0_ && y <= y1_ && txMouseButtons() == 1) {
		while (txMouseButtons() == 1) txSleep(100);
		return 1;
	}

	return 0;
}

double* generate_array(size_t size_of_array) {
	double* array = new double[size_of_array];

	srand(time(0));

	for (int i = 0; i < size_of_array; i++) array[i] = 1 + rand() % 1000;
	return array;
}

int get_number_of_comparisons(size_t size_of_array) {
	return (size_of_array * size_of_array - size_of_array) / 2;
}

int selection_sort(size_t size_of_array, double* array) {
	int number_of_swaps = 0;

	for (int start_index = 0; start_index < size_of_array - 1; start_index++) {
		int min_index = start_index;

		for (int current_index = start_index + 1; current_index < size_of_array; current_index++)
			if (array[current_index] < array[min_index]) min_index = current_index;

		std::swap(array[start_index], array[min_index]);

		if (start_index != min_index) number_of_swaps++;
	}

	return number_of_swaps;
}

int bubble_sort(size_t size_of_array, double* array) {
	int number_of_swaps = 0;

	for (int i = 0; i < size_of_array - 1; i++) {
		for (int j = 0; j < size_of_array - i - 1; j++) {
			if (array[j] > array[j + 1]) {
				std::swap(array[j], array[j + 1]);
				number_of_swaps++;
			}
		}
	}

	return number_of_swaps;
}

int* draw_sort(int kind_of_sort, double scaleX, double scaleY) {
	int end = 0;

	CoordSys left_graph(20, 20, 390, 450, scaleX, scaleY);
	CoordSys right_graph(410, 20, 789, 450, 0.08, 0.00008);

	int* numbers_of_swaps_bubble_sort = new int[300];
	int* numbers_of_swaps_selection_sort = new int[300];

	for (size_t size_of_array = 10; size_of_array < 3000; size_of_array += 10) {
		double* array = generate_array(size_of_array);
		int number_of_comparisons = get_number_of_comparisons(size_of_array), number_of_swaps = 0;

		if (kind_of_sort == 0) {
			number_of_swaps = bubble_sort(size_of_array, array);
			numbers_of_swaps_bubble_sort[size_of_array / 10] = number_of_swaps;

			txSetColor(TX_LIGHTBLUE);
		}
		else {
			number_of_swaps = selection_sort(size_of_array, array);
			numbers_of_swaps_selection_sort[size_of_array / 10] = number_of_swaps;

			txSetColor(TX_ORANGE);
		}

		if (!end) end = left_graph.draw_point(size_of_array, number_of_swaps);

		if (right_graph.draw_point(size_of_array, number_of_comparisons)) break;

		delete[] array;
	}

	if (kind_of_sort == 0) return numbers_of_swaps_bubble_sort;
	else return numbers_of_swaps_selection_sort;
}

Window left_field(20, 20, 390, 450, "dependence of the number of swaps\non the number of elements");
Window right_field(410, 20, 789, 450, "dependence of the number of comparisons\non the number of elements");

Button Bubble_sort(100, 500, 300, 550, "Bubble sort");
Button Clear(350, 500, 450, 550, "Clear");
Button Selection_sort(500, 500, 700, 550, "Selecton sort");
Button Exit(725, 500, 775, 550, "Exit");
Button Plus(20, 460, 40, 480, "+");
Button Minus(40, 460, 60, 480, "-");

void create_working_space() {
	left_field.draw_field();
	right_field.draw_field();

	Bubble_sort.draw_button();
	Clear.draw_button();
	Selection_sort.draw_button();
	Exit.draw_button();
}

void redraw(int* numbers_of_swaps, double scaleX, double scaleY) {
	CoordSys left_graph(20, 20, 390, 450, scaleX, scaleY);
	CoordSys right_graph(410, 20, 789, 450, 0.08, 0.00008);

	int end = 0;
	for (int size_of_array = 10; size_of_array < 3000; size_of_array += 10) {

		if (!end) if (left_graph.draw_point(size_of_array, numbers_of_swaps[size_of_array / 10])) end = 1;
		right_graph.draw_point(size_of_array, get_number_of_comparisons(size_of_array));
	}
}

int main() {
	int width = 800, height = 600;
	txCreateWindow(width, height);
	txSetColor(TX_BLACK);
	txClear();

	create_working_space();

	double scaleX = 0.1, scaleY = 0.01;

	int* numbers_of_swaps_selection_sort = new int[300];
	int* numbers_of_swaps_bubble_sort = new int[300];

	int bubble_plotted = 0, selection_plotted = 0;

	while (TRUE) {
		if (Plus.is_button_pressed()) {
			scaleY *= 2;

			txSetColor(TX_BLACK);
			left_field.draw_field();

			CoordSys left_graph(20, 20, 390, 450, scaleX, scaleY);

			if (bubble_plotted) {
				txSetColor(TX_LIGHTBLUE);
				left_graph.redraw(numbers_of_swaps_bubble_sort);
			}
			if (selection_plotted) {
				txSetColor(TX_ORANGE);
				left_graph.redraw(numbers_of_swaps_selection_sort);
			}

		}
		if (Minus.is_button_pressed()) {
			scaleY /= 2;

			txSetColor(TX_BLACK);
			left_field.draw_field();

			CoordSys left_graph(20, 20, 390, 450, scaleX, scaleY);

			if (bubble_plotted) {
				txSetColor(TX_LIGHTBLUE);
				left_graph.redraw(numbers_of_swaps_bubble_sort);
			}
			if (selection_plotted) {
				txSetColor(TX_ORANGE);
				left_graph.redraw(numbers_of_swaps_selection_sort);
			}
		}

		int kind_of_sort = -1;

		if (Bubble_sort.is_button_pressed()) {
			kind_of_sort = 0;
			numbers_of_swaps_bubble_sort = draw_sort(kind_of_sort, scaleX, scaleY);
			bubble_plotted = 1;

			txSetColor(TX_BLACK);
			Plus.draw_button();
			Minus.draw_button();
		}
		if (Selection_sort.is_button_pressed()) {
			kind_of_sort = 1;
			numbers_of_swaps_selection_sort = draw_sort(kind_of_sort, scaleX, scaleY);
			selection_plotted = 1;

			txSetColor(TX_BLACK);
			Plus.draw_button();
			Minus.draw_button();
		}

		if (Clear.is_button_pressed()) {
			txClear();

			if (bubble_plotted) {
				delete[] numbers_of_swaps_bubble_sort;
				int* numbers_of_swaps_bubble_sort = new int[300];
				bubble_plotted = 0;
			}
			if (selection_plotted) {
				delete[] numbers_of_swaps_selection_sort;
				int* numbers_of_swaps_selection_sort = new int[300];
				selection_plotted = 0;
			}

			txSetColor(TX_BLACK);
			create_working_space();
		}
		if (Exit.is_button_pressed()) break;
	}

	txDisableAutoPause();

	if (bubble_plotted) delete[] numbers_of_swaps_bubble_sort;
	if (selection_plotted) delete[] numbers_of_swaps_selection_sort;
}


