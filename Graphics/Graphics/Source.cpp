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
	void draw_numbers(int x);
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

	if (x < (x1_ - x0_ - 22) && y < (y1_ - y0_ - 22)) {
		txCircle(find_x(x), find_y(y), 2);
		return 0;
	}

	return 1;
}

double CoordSys::find_x(double x) {
	return x0_ + x + 20;
}

double CoordSys::find_y(double y) {
	return y1_ - y - 20;
}

void Window::draw_numbers(int i) { //ono rabotaet ne pravilno
	txSetColor(TX_BLACK);

	char number_x[5], number_y[8];
	_itoa_s(i * 10, number_x, 10);
	_itoa_s(i * 1000, number_y, 10);

	txSelectFont("System", false, false, false, false, false, false, 0);
	txDrawText(x0_ + 20 + 2 + i, 430, x1_ + 50 + 2 + i, 450, number_x, DT_VCENTER);

	txDrawText(x0_, y1_ - 80 - 2 - i, x0_ + 40, y1_ - 20 - 2 - i, number_y, DT_VCENTER);
}

void Window::draw_text() {
	txSelectFont("System", false, false, false, false, false, false, 0);

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

	txLine(x0_, y1_ - 20, x1_, y1_ - 20);
	txLine(x0_ + 20, y0_, x0_ + 20, y1_);

	txLine(x1_, y1_ - 20, x1_ - 5, y1_ - 20 - 5);
	txLine(x1_, y1_ - 20, x1_ - 5, y1_ - 20 + 5);

	txLine(x0_ + 20, y0_, x0_ + 20 + 5, y0_ + 5);
	txLine(x0_ + 20, y0_, x0_ + 20 - 5, y0_ + 5);

	for (int x = x0_ + 20; x < x1_; x += 30) txLine(x, y1_ - 20 - 1, x, y1_ - 20 + 2); 
	for (int y = y0_ + 20; y < y1_; y += 30) txLine(x0_ + 20 - 1, y, x0_ + 20 + 2, y);
}

void Window::draw_field() {
	txRectangle(x0_, y0_, x1_, y1_);
	draw_text();
	grid();
	axes();

	for (int i = 0; i <= 300; i += 60) draw_numbers(i);
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

void create_working_space(Window& left_field,
						  Window& right_field,
						  Button& Bubble_sort,
	                      Button& Clear,
	                      Button& Selection_sort,
	                      Button& Exit) {

	left_field.draw_field();
	right_field.draw_field();

	Bubble_sort.draw_button();
	Clear.draw_button();
	Selection_sort.draw_button();
	Exit.draw_button();
}

int main() {
	int width = 800, height = 600;
	txCreateWindow(width, height);
	txSetColor(TX_BLACK);
	txClear();

	Window left_field(20, 20, 390, 450, "dependence of the number of swaps\non the number of elements");
	Window right_field(410, 20, 789, 450, "dependence of the number of comparisons\non the number of elements");

	Button Bubble_sort(100, 500, 300, 550, "Bubble sort");
	Button Clear(350, 500, 450, 550, "Clear");
	Button Selection_sort(500, 500, 700, 550, "Selecton sort");
	Button Exit(725, 500, 775, 550, "Exit");
	Button Plus(20, 460, 40, 480, "+");
	Button Minus(40, 460, 60, 480, "-");

	create_working_space(left_field,
		                 right_field,
		                 Bubble_sort,
		                 Clear,
		                 Selection_sort,
		                 Exit);

	double scaleX = 0.1, scaleY = 0.01;

	int* numbers_of_swaps_selection_sort = new int[300];
	int* numbers_of_swaps_bubble_sort = new int[300];

	int bubble_plotted = 0, selection_plotted = 0;

	while (TRUE) {
		if (Plus.is_button_pressed() && (bubble_plotted || selection_plotted)) {
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
		if (Minus.is_button_pressed() && (bubble_plotted || selection_plotted)) {
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

			if (bubble_plotted) bubble_plotted = 0;
			if (selection_plotted) selection_plotted = 0;

			scaleX = 0.1;
			scaleY = 0.01;

			txSetColor(TX_BLACK);
			create_working_space(left_field,
				right_field,
				Bubble_sort,
				Clear,
				Selection_sort,
				Exit);
		}
		if (Exit.is_button_pressed()) break;
	}

	txDisableAutoPause();


	if (bubble_plotted) delete[] numbers_of_swaps_bubble_sort;
	if (selection_plotted) delete[] numbers_of_swaps_selection_sort;
}
