#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <deque>
#include <iostream>
#include <string>

class BigInt
{
	public:
		// Constructors
		BigInt(const std::string& str="");
		BigInt(int num);
		BigInt(const BigInt& other);
		virtual ~BigInt();

		// Public Methods
		unsigned int size() const;
		static BigInt karatsubaMult(const BigInt& a, const BigInt& b);

		// Operator Overloads
		BigInt& operator =(const BigInt& r);
		BigInt& operator +=(const BigInt& r);
		BigInt& operator -=(const BigInt& r);
		BigInt& operator *=(const BigInt& r);
		BigInt& operator *=(int r);
		BigInt& operator /=(const BigInt& r);
		BigInt& operator %=(const BigInt& r);
		
		// Friend Arithmetic Binary Operator Overloads
		friend BigInt operator +(const BigInt& l, const BigInt& r);
		friend BigInt operator -(const BigInt& l, const BigInt& r);
		friend BigInt operator *(const BigInt& l, const BigInt& r);
		friend BigInt operator *(const BigInt& l, int r);
		friend BigInt operator /(const BigInt& l, const BigInt& r);
		friend BigInt operator %(const BigInt& l, const BigInt& r);
		
		// Friend Unary Operator Overloads
		friend BigInt operator -(const BigInt& r);
		
		// Friend Boolean Binary Operator Overloads
		friend bool operator <(const BigInt& l, const BigInt& r);
		friend bool operator >(const BigInt& l, const BigInt& r);
		friend bool operator <=(const BigInt& l, const BigInt& r);
		friend bool operator >=(const BigInt& l, const BigInt& r);
		friend bool operator ==(const BigInt& l, const BigInt& r);
		friend bool operator !=(const BigInt& l, const BigInt& r);

		// Stream Operator Overloads
		friend std::ostream& operator <<(std::ostream& os, const BigInt& obj);
		friend std::istream& operator >>(std::istream& is, BigInt& obj);

		// Helpers
		inline int ascii2Int(char c) { return c-'0'; }
		static BigInt& padZeros(BigInt& bigInt, int numZeros);
		BigInt& trimLeadingZeros(BigInt& bigInt);
		BigInt& inputAndChangeFromBase10(BigInt& a, const std::string& s);
		
		BigInt abs() const;
		static bool isEven(const BigInt& b);
		std::string to_string();

		// Member Variables
		std::deque<int> digits;
		int base;
		int sign;
		static const int BASE = 10;
		static const int BASE_DIGITS = 1;

		enum Sign { NEGATIVE = -1, POSITIVE = 1 };
};

#endif