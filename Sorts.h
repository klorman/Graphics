#pragma once

#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <functional>

#include "EvilInt.h"

template<typename Type>
class Sorts {
private:
	Type*                                    _arr;
	int                                      _size;
	std::vector<std::function<void(Sorts&)>> _sortsVec;

	int  partition(int low, int high);
	void recQuickSort(int low, int high);
	void countSort(Type exp);
	Type getMax();
	void shuffle();
	bool isSorted();

public:
	Sorts();
	~Sorts();

	void generateArray(int size);
	void bubbleSort();
	void selectionSort();
	void quickSort();
	void radixSort();
	void bogoSort();

	int getNumberOfComparisons();
	int	getNumberOfAssignments();

	void startSort(GRAPHICS id);
	void printArray();
};


template<typename Type>
Sorts<Type>::Sorts() :
	_arr  (nullptr),
	_size (0),
	_sortsVec({})
{ 
	_sortsVec.push_back(&Sorts::bubbleSort);
	_sortsVec.push_back(&Sorts::selectionSort);
	_sortsVec.push_back(&Sorts::quickSort);
	_sortsVec.push_back(&Sorts::radixSort);
	_sortsVec.push_back(&Sorts::bogoSort);
}

template<typename Type>
Sorts<Type>::~Sorts() {
	if (_arr)
		delete[] _arr;
}

template<typename Type>
void Sorts<Type>::generateArray(int size) {
	assert(size > 0);

	if (_arr != nullptr)
		delete[] _arr;

	_arr = new Type[size];
	_size = size;

	srand((unsigned int) time(0));

	for (int i = 0; i < size; ++i)
		_arr[i] = 1 + rand() % 1000;
}

template<typename Type>
void Sorts<Type>::bubbleSort() {
	assert(_arr);
	_arr[0].clearStatistics();

	for (int i = 0; i < _size - 1; ++i) {
		for (int j = 0; j < _size - i - 1; ++j) {
			if (_arr[j] > _arr[j + 1]) {
				std::swap(_arr[j], _arr[j + 1]);
			}
		}
	}
}

template<typename Type>
void Sorts<Type>::selectionSort() {
	assert(_arr);
	_arr[0].clearStatistics();

	int min_idx = 0;

	for (int i = 0; i < _size - 1; ++i) {
		min_idx = i;

		for (int j = i + 1; j < _size; ++j)
			if (_arr[j] < _arr[min_idx])
				min_idx = j;

		std::swap(_arr[min_idx], _arr[i]);
	}
}

template<typename Type>
void Sorts<Type>::recQuickSort(int low, int high) {
	assert(low >= 0);

	if (low < high) {
		int pi = partition(low, high);

		recQuickSort(low, pi - 1);
		recQuickSort(pi + 1, high);
	}
}

template<typename Type>
void Sorts<Type>::quickSort() {
	assert(_arr);
	_arr[0].clearStatistics();

	recQuickSort(0, _size - 1);
}

template<typename Type>
int Sorts<Type>::partition(int low, int high) {
	assert(low >= 0 && high > low);

	int i = low - 1;

	for (int j = low; j <= high - 1; ++j)
		if (_arr[j] < _arr[high])
			std::swap(_arr[++i], _arr[j]);

	std::swap(_arr[i + 1], _arr[high]);

	return ++i;
}

template<typename Type>
void Sorts<Type>::radixSort() {
	assert(_arr);
	_arr[0].clearStatistics();

	Type m = getMax();

	for (Type exp = 1; int(m / exp) > 0; exp *= 10) {
		countSort(exp);
	}
}

template<typename Type>
void Sorts<Type>::countSort(Type exp) {
	Type* output = new Type[_size];
	int  count[10] = {};

	for (int i = 0; i < _size; ++i)
		count[int(_arr[i] / exp) % 10]++;

	for (int i = 1; i < 10; ++i)
		count[i] += count[i - 1];

	for (int i = _size - 1; i >= 0; --i) {
		output[count[(int)(_arr[i] / exp) % 10] - 1] = _arr[i];
		count[(int)(_arr[i] / exp) % 10]--;
	}

	std::swap(_arr, output);

	delete[] output;
}

template<typename Type>
Type Sorts<Type>::getMax() {
	Type mx = _arr[0];
	
	for (int i = 1; i < _size; ++i)
		if (_arr[i] > mx)
			mx = _arr[i];

	return mx;
}

template<typename Type>
void Sorts<Type>::bogoSort() {
	assert(_arr);
	_arr[0].clearStatistics();

	while (!isSorted())
		shuffle();
}

template<typename Type>
void Sorts<Type>::shuffle() {
	for (int i = 0; i < _size; ++i)
		std::swap(_arr[i], _arr[rand() % _size]);
}

template<typename Type>
bool Sorts<Type>::isSorted() {
	for (int i = 1; i < _size; ++i)
		if (_arr[i] < _arr[i - 1])
			return false;

	return true;
}

template<typename Type>
int	Sorts<Type>::getNumberOfComparisons() {
	int numberOfComparisons = 0;

	for (int i = 0; i < _size; ++i)
		numberOfComparisons += _arr[i].getNumberOfComparisons();

	return numberOfComparisons;
}

template<typename Type>
int	Sorts<Type>::getNumberOfAssignments() {
	int numberOfAssignments = 0;

	for (int i = 0; i < _size; ++i)
		numberOfAssignments += _arr[i].getNumberOfAssignments();

	return numberOfAssignments;
}

template<typename Type>
void Sorts<Type>::startSort(GRAPHICS id) {
	_sortsVec[id](*this);
}

template<typename Type>
void Sorts<Type>::printArray() {
	assert(_arr);

	for (int i = 0; i < _size; ++i)
		printf("%d ", (int) _arr[i]);

	printf("\n");
}