#include "TXLib.h"

#include <iostream> 
#include <algorithm>
#include <ctime>
#include <fstream>

struct Coords {
	int x0, y0, x1, y1;
};

struct Fields {
	Coords field_l;
	Coords field_r;
};

struct Graph {
	Coords left, right;
};

void create_grid(Coords const field) {
	txSetColor(TX_LIGHTGRAY);

	for (int x = field.x0; x < field.x1; x += 10) txLine(x, field.y0, x, field.y1);
	for (int y = field.y0; y < field.y1; y += 10) txLine(field.x0, y, field.x1, y);
}

void create_axes(Coords const field, int width, int height) {
	create_grid(field);

	txSetColor(TX_BLACK);

	txLine(field.x0, field.y1 - 10, field.x1, field.y1 - 10);
	txLine(field.x0 + 10, field.y0, field.x0 + 10, field.y1);

	txLine(field.x1, field.y1 - 10, field.x1 - 5, field.y1 - 10 - 5);
	txLine(field.x1, field.y1 - 10, field.x1 - 5, field.y1 - 10 + 5);

	txLine(field.x0 + 10, field.y0, field.x0 + 10 + 5, field.y0 + 5);
	txLine(field.x0 + 10, field.y0, field.x0 + 10 - 5, field.y0 + 5);

	for (int x = field.x0; x < field.x1; x += 10) txLine(x, field.y1 - 10 - 1, x, field.y1 - 10 + 2);
	for (int y = field.y0; y < field.y1; y += 10) txLine(field.x0 + 10 - 1, y, field.x0 + 10 + 2, y);
}

void create_field(int width, int height, Coords field) {
	txRectangle(field.x0, field.y0, field.x1, field.y1);

	create_axes(field, width, height);
}

void create_button(Coords button, const char* text) {
	txRectangle(button.x0, button.y0, button.x1, button.y1);
	txDrawText(button.x0, button.y0, button.x1, button.y1, text);
}

Fields create_fields(int width, int height) {
	Coords field_l = { 20, 20, width / 2 - 10, height - 150 };

	create_field(width, height, field_l);

	Coords field_r = { width / 2 + 10, 20, width - 20, height - 150 };

	create_field(width, height, field_r);

	Coords button;
	
	button = { 100, height - 100, 300, height - 50 };
	create_button(button, "Bubble sort");

	button = { width - 300, height - 100, width - 100, height - 50 };
	create_button(button, "Selection sort");

	button = { 350, height - 100, 450, height - 50 };
	create_button(button, "Clear");

	Fields fields = { field_l, field_r };
	return fields;
}

int if_button_pressed(int width, int height) {
	while (TRUE) {
		if (txMouseButtons() == 1) {
			int x = txMouseX(), y = txMouseY();

			if (x >= 100 && x <= 300 && y >= height - 100 && y <= height - 50) return 0;                      //Bubble sort
			if (x >= width - 300 && x <= width - 100 && y >= height - 100 && y <= height - 50) return 1;      //Selection sort
			if (x >= 350 && x <= 450 && y >= height - 100 && y <= height - 50) return 2;                      //Clear
		}
	}
}

void plot_point(Coords line, Coords field, double scale_x, double scale_y) {
	double x = line.x1 * scale_x, y = line.y1 * scale_y;

	if (x < (field.x1 - field.x0) && y < (field.y1 - field.y0)) txCircle(x + field.x0 + 10, field.y1 - y - 10, 2);
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

void selection_sort(size_t size_of_array, double* array, Fields fields, Graph gr) {
	int number_of_comparisons = get_number_of_comparisons(size_of_array), number_of_swaps = 0;

	for (int start_index = 0; start_index < size_of_array - 1; start_index++) {
		int min_index = start_index;

		for (int current_index = start_index + 1; current_index < size_of_array; current_index++)
			if (array[current_index] < array[min_index]) min_index = current_index;

		std::swap(array[start_index], array[min_index]);

		if (start_index != min_index) number_of_swaps++;
	}

	gr.left.x1 = size_of_array;
	gr.left.y1 = number_of_swaps;

	plot_point(gr.left, fields.field_l, 0.1, 0.01);

	gr.right.x1 = size_of_array;
	gr.right.y1 = number_of_comparisons;

	plot_point(gr.right, fields.field_r, 0.08, 0.00008);
}
void bubble_sort(size_t size_of_array, double* array, Fields fields, Graph gr) {
	int number_of_comparisons = get_number_of_comparisons(size_of_array), number_of_swaps = 0;

	for (int i = 0; i < size_of_array - 1; i++) {
		for (int j = 0; j < size_of_array - i - 1; j++) {
			if (array[j] > array[j + 1]) {
				std::swap(array[j], array[j + 1]);
				number_of_swaps++;
			}
		}
	}

	gr.left.x1 = size_of_array;
	gr.left.y1 = number_of_swaps;

	plot_point(gr.left, fields.field_l, 0.1, 0.0001);

	gr.right.x1 = size_of_array;
	gr.right.y1 = number_of_comparisons;

	plot_point(gr.right, fields.field_r, 0.08, 0.00008);
}



int main() {
	int width = 800, height = 600;
	txCreateWindow(width, height);
	txSetColor(TX_BLACK);
	txClear();

	Fields fields = create_fields(width, height);

	while (TRUE) {
		Graph gr = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
		int button = if_button_pressed(800, 600);

		txSetColor(TX_LIGHTBLUE);

		if (button == 0) {
			for (size_t size_of_array = 10; size_of_array < 3000; size_of_array += 10) {
				double* array = generate_array(size_of_array);

				bubble_sort(size_of_array, array, fields, gr);

				delete[] array;
			}
		}

		if (button == 1) {
			for (size_t size_of_array = 10; size_of_array < 3000; size_of_array += 10) {
				double* array = generate_array(size_of_array);

				selection_sort(size_of_array, array, fields, gr);

				delete[] array;
			}
		}

		if (button == 2) {
			txClear();
			txSetColor(TX_BLACK);
			Fields fields = create_fields(width, height);
		}

		std::cout << "Done! ";
		txSleep(1000);
	}
}


