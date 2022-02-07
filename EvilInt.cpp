#pragma once

#include <cassert>
#include "EvilInt.h"

int EvilInt::_numberOfAssignments = 0;
int EvilInt::_numberOfComparisons = 0;

EvilInt::EvilInt(int i /*= 0*/) :
	_value(i)
{}

EvilInt& EvilInt::operator = (const EvilInt& right) {
	if (&this->_value == &right._value)
		return *this;

	_value = right._value;
	++_numberOfAssignments;

	return *this;
}

const EvilInt& operator + (const EvilInt& right) {
	return right._value;
}

const EvilInt operator - (const EvilInt& right) {
	return -right._value;
}

const EvilInt operator + (const EvilInt& left, const EvilInt& right) {
	return left._value + right._value;
}

const EvilInt operator - (const EvilInt& left, const EvilInt& right) {
	return left + (-right);
}

const EvilInt operator * (const EvilInt& left, const EvilInt& right) {
	return left._value * right._value;
}

const EvilInt operator / (const EvilInt& left, const EvilInt& right) {
	assert(right._value != 0);

	return left._value / right._value;
}

const EvilInt operator % (const EvilInt& left, const EvilInt& right) {
	return left._value % right._value;
}

const EvilInt& operator ++ (EvilInt& right) {
	right._value++;

	return right;
}

const EvilInt  operator ++ (EvilInt& right, int) {
	EvilInt old_right = right;
	right._value++;

	return old_right;
}

const EvilInt& operator -- (EvilInt& right) {
	right._value--;

	return right;
}

const EvilInt  operator -- (EvilInt& right, int) {
	EvilInt old_right = right;
	right._value--;

	return old_right;
}

const EvilInt operator ^ (const EvilInt& left, const EvilInt& right) {
	return left._value ^ right._value;
}

const EvilInt operator & (const EvilInt& left, const EvilInt& right) {
	return left._value & right._value;
}

const EvilInt operator | (const EvilInt& left, const EvilInt& right) {
	return left._value | right._value;
}

const EvilInt operator ~ (const EvilInt& right) {
	return ~right._value;
}

const EvilInt operator << (const EvilInt& left, const EvilInt& right) {
	return left._value << right._value;
}

const EvilInt operator >> (const EvilInt& left, const EvilInt& right) {
	return left._value >> right._value;
}

const EvilInt operator ! (const EvilInt& right) {
	return !right._value;
}

const EvilInt operator && (const EvilInt& left, const EvilInt& right) {
	return left._value && right._value;
}

const EvilInt operator || (const EvilInt& left, const EvilInt& right) {
	return left._value || right._value;
}

bool operator >  (EvilInt& left, EvilInt& right) {
	++left._numberOfComparisons;
	
	return left._value > right._value;
}

bool operator <  (EvilInt& left, EvilInt& right) {
	++left._numberOfComparisons;

	return left._value < right._value;
}

bool operator >= (EvilInt& left, EvilInt& right) {
	++left._numberOfComparisons;

	return left._value >= right._value;
}

bool operator <= (EvilInt& left, EvilInt& right) {
	++left._numberOfComparisons;

	return left._value <= right._value;
}

bool operator == (EvilInt& left, EvilInt& right) {
	++left._numberOfComparisons;

	return left._value == right._value;
}

bool operator != (EvilInt& left, EvilInt& right) {
	++left._numberOfComparisons;

	return left._value != right._value;
}

EvilInt& operator += (EvilInt& left, const EvilInt& right) {
	return left = left + right;
}

EvilInt& operator -= (EvilInt& left, const EvilInt& right) {
	return left = left - right;
}

EvilInt& operator *= (EvilInt& left, const EvilInt& right) {
	return left = left * right;
}

EvilInt& operator /= (EvilInt& left, const EvilInt& right) {
	assert(right._value != 0);

	return left = left / right;
}

EvilInt& operator %= (EvilInt& left, const EvilInt& right) {
	return left = left % right;
}

EvilInt& operator &= (EvilInt& left, const EvilInt& right) {
	return left = left & right;
}

EvilInt& operator |= (EvilInt& left, const EvilInt& right) {
	return left = left | right;
}

EvilInt& operator ^= (EvilInt& left, const EvilInt& right) {
	return left = left ^ right;
}

EvilInt& operator <<= (EvilInt& left, const EvilInt& right) {
	return left = left << right;
}

EvilInt& operator >>= (EvilInt& left, const EvilInt& right) {
	return left = left >> right;
}

void EvilInt::clearStatistics() {
	_numberOfAssignments = 0;
	_numberOfComparisons = 0;
}

int EvilInt::getNumberOfAssignments() {
	return _numberOfAssignments;
}

int EvilInt::getNumberOfComparisons() {
	return _numberOfComparisons;
}