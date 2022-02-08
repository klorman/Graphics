#pragma once

struct EvilInt {
private:
	int _value;
	static int _numberOfComparisons,
        	   _numberOfAssignments;

public:
	EvilInt(int i = 0);

	EvilInt& operator =  (const EvilInt& right);

	friend const EvilInt& operator + (const EvilInt& i);
	friend const EvilInt  operator - (const EvilInt& i);

	friend const EvilInt& operator ++ (EvilInt& i);
	friend const EvilInt  operator ++ (EvilInt& i, int);
	friend const EvilInt& operator -- (EvilInt& i);
	friend const EvilInt  operator -- (EvilInt& i, int);

	friend const EvilInt  operator + (const EvilInt& left, const EvilInt& right);
	friend const EvilInt  operator - (const EvilInt& left, const EvilInt& right);
	friend const EvilInt  operator * (const EvilInt& left, const EvilInt& right);
	friend const EvilInt  operator / (const EvilInt& left, const EvilInt& right);
	friend const EvilInt  operator % (const EvilInt& left, const EvilInt& right);

	friend EvilInt& operator += (EvilInt& left, const EvilInt& right);
	friend EvilInt& operator -= (EvilInt& left, const EvilInt& right);
	friend EvilInt& operator *= (EvilInt& left, const EvilInt& right);
	friend EvilInt& operator /= (EvilInt& left, const EvilInt& right);
	friend EvilInt& operator %= (EvilInt& left, const EvilInt& right);
	friend EvilInt& operator &= (EvilInt& left, const EvilInt& right);
	friend EvilInt& operator |= (EvilInt& left, const EvilInt& right);
	friend EvilInt& operator ^= (EvilInt& left, const EvilInt& right);

	friend const EvilInt operator ^  (const EvilInt& left, const EvilInt& right);
	friend const EvilInt operator &  (const EvilInt& left, const EvilInt& right);
	friend const EvilInt operator |  (const EvilInt& left, const EvilInt& right);
	friend const EvilInt operator ~  (const EvilInt& right);
	friend const EvilInt operator << (const EvilInt& left, const EvilInt& right);
	friend const EvilInt operator >> (const EvilInt& left, const EvilInt& right);

	friend const EvilInt operator !  (const EvilInt& right);
	friend const EvilInt operator && (const EvilInt& left, const EvilInt& right);
	friend const EvilInt operator || (const EvilInt& left, const EvilInt& right);

	friend bool operator >  (EvilInt& left, EvilInt& right);
	friend bool operator <  (EvilInt& left, EvilInt& right);
	friend bool operator == (EvilInt& left, EvilInt& right);
	friend bool operator != (EvilInt& left, EvilInt& right);
	friend bool operator >= (EvilInt& left, EvilInt& right);
	friend bool operator <= (EvilInt& left, EvilInt& right);

	friend EvilInt& operator <<= (EvilInt& left, const EvilInt& rigth);
	friend EvilInt& operator >>= (EvilInt& left, const EvilInt& rigth);

	explicit operator int() const {
		return _value;
	}

	void clearStatistics();
	int  getNumberOfComparisons();
	int	 getNumberOfAssignments();
};