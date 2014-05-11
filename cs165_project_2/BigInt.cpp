#include "BigInt.hpp"
#include <cmath>
#include <sstream>
#include <stdexcept>

// Constructors
BigInt::BigInt(const std::string& str)
{
	int startPos = 0;
	if (str.compare(0,1,"-") == 0) {
		sign = NEGATIVE;
		startPos = 1;
	} else {
		sign = POSITIVE;
	}

	for (unsigned int i=startPos; i < str.length(); ++i)
		digits.push_front(ascii2Int(str[i]));
	base = BASE;
}

BigInt::BigInt(int num)
{
	int i=0;
	base = BASE;
	while (num > 0) {
		digits.push_back(num % base);
		num = std::floor(num / base);
		++i;
	}

	if (num > 0)
		sign = POSITIVE;
	else
		sign = NEGATIVE;
}

BigInt::BigInt(const BigInt& other)
{
	base = other.base;
	sign = other.sign;
	for (unsigned int i=0; i < other.size(); ++i)
		digits.push_back(other.digits[i]);
}

BigInt::~BigInt()
{
	digits.clear();
}

// Public Methods
unsigned int BigInt::size() const
{
	return digits.size();
}

BigInt BigInt::karatsubaMult(const BigInt& a, const BigInt& b)
{
	int minSize = std::min(a.size(), b.size());
	if (minSize < 30) {
		return a * b;
	}

	BigInt x0, x1, y0, y1;
	BigInt z0, z1, z2;
	int mid = minSize / 2;

	x0.digits.resize(mid);
	x1.digits.resize(a.digits.size() - mid);

	y0.digits.resize(mid);
	y1.digits.resize(b.digits.size() - mid);

	copy(a.digits.begin(), a.digits.begin() + mid, x0.digits.begin());
	copy(a.digits.begin() + mid, a.digits.end(), x1.digits.begin());

	copy(b.digits.begin(), b.digits.begin() + mid, y0.digits.begin());
	copy(b.digits.begin() + mid, b.digits.end(), y1.digits.begin());

	z0 = karatsubaMult(x0, y0);
	z1 = karatsubaMult(x1, y1);
	z2 = (karatsubaMult(x0 + x1, y0 + y1) - z1) - z0;

	padZeros(z2, mid);
	padZeros(z1, 2*mid);

	return z0 + z1 + z2;
}

// Operator Overloads
BigInt& BigInt::operator =(const BigInt& r)
{
	if (this != &r) {
		this->digits = r.digits;
		this->base = r.base;
		this->sign = r.sign;
	}
	return *this;
}

BigInt& BigInt::operator +=(const BigInt& r)
{
	/*
	switch(r.sign) {
		case NEGATIVE :
			if (this->sign == POSITIVE)
				return *this -= r;
			break;
		case POSITIVE :
			if (this->sign == NEGATIVE) 
				return *this = r - *this;
			break;
	}
	*/

	if ( this->sign == r.sign ) 
	{
		BigInt rhs(r);
		if (digits.size() < rhs.size())
			digits.resize(rhs.size());
		if (digits.size() > rhs.size())
			rhs.digits.resize(digits.size());
		
		int num = 0, carry = 0;

		for (unsigned int i=0; i < rhs.size(); ++i) {
			num = this->digits[i] + rhs.digits[i] + carry;
			if (num >= base) {
				num -= base;
				carry = 1;
			}
			else {
				carry = 0;
			}
			this->digits[i] = num;
		}

		if (carry) {
			this->digits.push_back(carry);
		}

		return *this;
	}
	*this = *this - (-r);
	return *this;
}

BigInt& BigInt::operator -=(const BigInt& r)
{
	/*
	switch(r.sign) {
		case NEGATIVE :
			if (this->sign == NEGATIVE)
				return *this += r;
			break;
		case POSITIVE :
			if (this->sign == NEGATIVE)
				return *this += r;
			break;
	}
	*/
	
	if ( this->sign == r.sign )
	{

		if ( this->abs() >= r.abs()) {
			BigInt rhs(r);
			if (digits.size() < rhs.size())
				digits.resize(rhs.size());
			if (digits.size() > rhs.size())
				rhs.digits.resize(digits.size());


			int num = 0, carry = 0;

			for (unsigned int i=0; i < rhs.size(); ++i) {
				num = this->digits[i] - rhs.digits[i] - carry;
				if (num < 0) {
					num += base;
					carry = 1;
				}
				else {
					carry = 0;
				}
				this->digits[i] = num;
			}
			if (carry) {
				this->digits.push_back(carry);
			}
			
			trimLeadingZeros(*this);
			return *this;
		}

		*this = -(r - *this);
		return *this;
	}
	*this = *this + (-r);
	return *this;
}

BigInt& BigInt::operator *=(const BigInt& r)
{
	if (!r.size()) {
		this->digits.clear();
		return *this;
	}
/*
	int carry_sign = this->sign * r.sign;
	this->sign = POSITIVE;
	BigInt rr = r.abs(); 
*/
	int num = 0, carry = 0;
	std::deque<int> largeDeq, smallDeq;

	if (this->size() > r.size()) {
		largeDeq = this->digits;
		smallDeq = r.digits;
	}
	else {
		largeDeq = r.digits;
		smallDeq = this->digits;
	}

	// Multiply without carrying
	BigInt* bArray = new BigInt[smallDeq.size()];
	for (unsigned int i=0; i < smallDeq.size(); ++i) {
		bArray[i].digits.resize(largeDeq.size());
		for(unsigned int j=0; j < largeDeq.size(); ++j) {
			bArray[i].digits[j] = largeDeq[j] * smallDeq[i];
		}
	}


	*this = bArray[0];
	digits.resize(smallDeq.size() + largeDeq.size());

	// Add without carrying
	for (unsigned int i=1; i < smallDeq.size(); ++i) {
		padZeros(bArray[i], i);
		for (unsigned int j=0; j < bArray[i].size(); ++j) {
			this->digits[j] += bArray[i].digits[j];
		}
	}

	// Perform carrying
	for (unsigned int i=0; i < digits.size(); ++i) {
		num = digits[i] + carry;
		carry = num / base;
		num %= base;
		digits[i] = num;
	}
	while (carry) {
		this->digits.push_back(carry%base);
		carry /= base;
	}

	trimLeadingZeros(*this);

/*
	if ( this->size() > 1 )
	{
		this->sign = carry_sign;
	}
*/
	delete [] bArray;
	
	return *this;
}

BigInt& BigInt::operator *=(int r)
{
	for (int i=0, carry=0; i < (int)this->size() || carry; ++i) {
		if (i == (int)this->size())
			this->digits.push_back(0);
		int cur = this->digits[i] * r + carry;
		carry = cur / this->base;
		this->digits[i] = cur % this->base;
	}
	
	trimLeadingZeros(*this);
	
	if (r < 0)
		this->sign = this->sign * -1;

	return *this;
}

BigInt& BigInt::operator /=(const BigInt& r)
{
	if (!r.size())
		throw std::invalid_argument("Divide by zero!");

/*
	int carry_sign = this->sign * r.sign;
	this->sign = POSITIVE;
	BigInt rr = r.abs();
*/
	BigInt toDivide;
	BigInt quotient;
	for (int i = this->size()-1; i >= 0; --i) {
		toDivide.digits.push_front(this->digits[i]);
		int q1 = toDivide.size() < r.size() ? 0 : 1;
		if (q1) {
			BigInt divisor = r;
			int qDiv = 0;
			while (divisor <= toDivide) {
				divisor += r;
				qDiv++;
			}
			if (qDiv) {
				toDivide -= (r * qDiv);
				trimLeadingZeros(toDivide);
				quotient.digits.push_front(qDiv);
			} else if (quotient.size()) {
				quotient.digits.push_front(0);
			}
		}
		else if (quotient.size()) {
			quotient.digits.push_front(0);
		}
	}

	//this->sign = carry_sign;

	if (r <= toDivide && quotient.digits[0] == 0) {
		BigInt divisor = r;
		int qDiv = 0;
		while (divisor <= toDivide) {
			divisor += r;
			qDiv++;
		}
		if (qDiv) {
			quotient.digits[0] = qDiv;
		}
	}
	else if ( quotient.size() == 0 ) {
		quotient.digits.push_back(0);
		quotient.sign = POSITIVE;
	}

	this->digits = quotient.digits;
	return *this;
}

BigInt& BigInt::operator %=(const BigInt& r)
{
	if (!r.size())
		throw std::invalid_argument("Divide by zero!");
/*
	int carry_sign = this->sign * r.sign;
	this->sign = POSITIVE;
	BigInt rr = r.abs();
*/
	BigInt toDivide;
	BigInt quotient;
	for (int i = this->size()-1; i >= 0; --i) {
		toDivide.digits.push_front(this->digits[i]);
		int q1 = toDivide.size() < r.size() ? 0 : 1;
		if (q1) {
			BigInt divisor = r;
			int qDiv = 0;
			while (divisor <= toDivide) {
				divisor += r;
				qDiv++;
			}
			if (qDiv) {
				toDivide -= (r * qDiv);
				trimLeadingZeros(toDivide);
				quotient.digits.push_front(qDiv);
			} else if (quotient.size()) {
				quotient.digits.push_front(0);
			}
		}
		else if (quotient.size()) {
			quotient.digits.push_front(0);
		}
	}

	//this->sign = carry_sign;

	if (r <= toDivide && quotient.digits[0] == 0) {
		BigInt divisor = r;
		int qDiv = 0;
		while (divisor <= toDivide) {
			divisor += r;
			qDiv++;
		}
		if (qDiv) {
			quotient.digits[0] = qDiv;
		}
	}
	else if ( quotient.size() == 0 ) {
		quotient.digits.push_back(0);
		quotient.sign = POSITIVE;
	}

	
	this->digits = toDivide.digits;
	return *this;
}

// Friend Arithmetic Binary Operator Overloads
BigInt operator +(const BigInt& l, const BigInt& r)
{
	BigInt result = l;
	return result += r;
}

BigInt operator -(const BigInt& l, const BigInt& r)
{
	BigInt result = l;
	return result -= r;
}

BigInt operator *(const BigInt& l, const BigInt& r)
{
	int carry_sign = l.sign * r.sign;
	BigInt result = l.abs();
	BigInt rr = r.abs();

	result *= rr;

	if ( result.size() > 1 )
	{
		result.sign = carry_sign;
	}
	
	return result;
}

BigInt operator *(const BigInt&l, int r)
{
	BigInt result = l;
	return result *= r;
}

BigInt operator /(const BigInt& l, const BigInt& r)
{
	int carry_sign = l.sign * r.sign;
	BigInt result = l.abs();
	BigInt rr = r.abs();

	result /= rr;

	if ( result.size() > 1 )
	{
		result.sign = carry_sign;
	}
	
	return result;
}

BigInt operator %(const BigInt& l, const BigInt& r)
{

	BigInt result = l;
	BigInt rr = r;

	result %= rr;
	result += rr;
	result %= rr;
	
	return result;
}

// Friend Unary Operator Overloads
BigInt operator -(const BigInt& r)
{
	BigInt result = r;
	result.sign = -r.sign;
	return result;
}

// Friend Boolean Binary Operator Overloads
bool operator <(const BigInt& l, const BigInt& r)
{
	if (l.size() < r.size())
		return true;
	if (l.size() > r.size())
		return false;

	for (int i=l.size()-1; i >= 0; --i) {
		if (l.digits[i] < r.digits[i]) 
			return true;
		if (l.digits[i] > r.digits[i])
			return false;
	}
	return false;
}

bool operator >(const BigInt& l, const BigInt& r)
{
	if (l.size() > r.size())
		return true;
	if (l.size() < r.size())
		return false;

	for (int i=l.size()-1; i >= 0; --i) {
		if (l.digits[i] < r.digits[i])
			return false;
		if (l.digits[i] > r.digits[i])
			return true;
	}
	return false;
}

bool operator <=(const BigInt& l, const BigInt& r)
{
	if (l.size() < r.size())
		return true;
	if (l.size() > r.size())
		return false;

	for (int i=l.size()-1; i >=0; --i) {
		if (l.digits[i] < r.digits[i])
			return true;
		if (l.digits[i] > r.digits[i])
			return false;
	}

	return true;
}

bool operator >=(const BigInt& l, const BigInt& r)
{
	if (l.size() > r.size())
		return true;
	if (l.size() < r.size())
		return false;

	for (int i=l.size()-1; i >=0; --i) {
		if (l.digits[i] > r.digits[i])
			return true;
		if (l.digits[i] < r.digits[i])
			return false;
	}

	return true;
}

bool operator ==(const BigInt& l, const BigInt& r)
{
	if (l.size() != r.size())
		return false;

	for (int i=l.size()-1; i >= 0; --i) {
		if (l.digits[i] != r.digits[i])
			return false;
	}
	return true;
}

bool operator !=(const BigInt& l, const BigInt& r)
{
	if (l.size() != r.size())
		return true;

	for (int i=l.size()-1; i >= 0; --i) {
		if (l.digits[i] != r.digits[i])
			return true;
	}
	return false;
}

// Stream Operator Overloads
std::ostream& operator <<(std::ostream& os, const BigInt& obj)
{
	if (obj.sign == BigInt::NEGATIVE)
		os << "-";
	for (int i=obj.size()-1; i >=0 ; --i)
		os << obj.digits[i];
	return os;
}

std::istream& operator >>(std::istream& is, BigInt& obj)
{
	std::string tmp;
	is >> tmp;
	if (tmp.compare(0,1,"-") == 0) {
		obj.sign = BigInt::NEGATIVE;
		tmp.erase(0,1);
	}
	obj.digits.clear();
	for (unsigned int i=0; i < tmp.length(); ++i)
		obj.digits.push_front(obj.ascii2Int(tmp[i]));
	return is;
}

// Helpers
BigInt& BigInt::padZeros(BigInt& bigInt, int numZeros)
{
	for (int i=0; i < numZeros; ++i)
		bigInt.digits.push_front(0);
	return bigInt;
}

BigInt& BigInt::trimLeadingZeros(BigInt& bigInt)
{
	for (int i=bigInt.size()-1; bigInt.digits[i] == 0 && i >= 1; --i)
		bigInt.digits.pop_back();
	return bigInt;
}

BigInt& BigInt::inputAndChangeFromBase10(BigInt& a, const std::string& s)
{
	for (int i = s.size()-1; i >= 0; i -= BASE_DIGITS) {
		int x = 0;
		for (int j = i - BASE_DIGITS + 1; j <= i; j++)
			x = x * 10 + s[j] - '0';
		a.digits.push_front(x);
	}
	std::cout << "A = " << a << std::endl;
	return a;
}

BigInt BigInt::abs() const {
	BigInt res = *this;
	res.sign *= res.sign;
	return res;
}

bool BigInt::isEven(const BigInt& b)
{
	return !(b.digits[0] % 2);
}

std::string BigInt::to_string()
{
	std::stringstream ss;
	for (int i = digits.size()-1; i >= 0; --i)
		ss << digits[i] << "";
	return ss.str();
}