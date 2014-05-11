#include "utility.hpp"

using namespace std;


// for normal integer part
bool is_probably_prime(int n) {
	if ( n % 2 == 0 )
		return false;

	int test_count;
	int b;

	int k = 20;

	while ( test_count < k ) {
		b = (int) ((n - 2) * dshrandom(0) + 1);

		if ( binary_GCD(b, n) != 1 )
		{ return false; }
		else if ( binary_GCD(b, n) == 1 && J(b, n) == modular_equivalent(b, n) ) {
			test_count++;
		}

	}

	return true;

}

int next_prime(int n)
{
	int right = ((n % 2 == 0) ? (n + 1) : (n + 2));
	while ( is_probably_prime(right) == 0 )
	{ right += 2; }
	return right;
}

// Euclid's GCD function
int GCD(int x, int y)
{
	while ( x != y ) {
		if ( x > y ) {
			x -= y;
		}
		else {
			y -= x;
		}
	}

	return x;
}

int b_GCD(int x, int y)
{
	if ( x < y)
		return b_GCD(y,x);
	if ( y == 0)
		return x;
	if ( x % 2 == 0 )
	{
		if ( y % 2 == 0)
		{
			return (2 * b_GCD(x / 2, y / 2));
		}
		else
		{
			return b_GCD(x / 2, y);
		}
	} else {
		if ( y % 2 == 0)
		{
			return b_GCD(x, y / 2);
		}
		else
		{
			return b_GCD(y, x-y);
		}
	}
}

// binary GCD function
unsigned int binary_GCD(unsigned int x, unsigned int y)
{
	if ( x == y) { return x; }
	if ( x == 0) { return y; }
	if ( y == 0) { return x; }
	if ( ~x & 1 ) { 
		return ((y & 1) ? binary_GCD(x >> 1, y) : (binary_GCD(x >> 1, y >> 1) << 1)); 
	}
	if ( ~y & 1 ) {
		return binary_GCD(x, y >> 1);
	}
	if ( x > y) { 
		return binary_GCD((x - y) >> 1, y);
	} else {
		return binary_GCD((y - x) >> 1, x);
	}
}

// Jacob Function
int J(int x, int y)
{
	if ( x == 1 ) {
		return 1;
	} else if ( x % 2 == 0 ) {
		int p = (y * y - 1) / 8;
		return (( p % 2 == 0) ? J(x/2, y) : -J(x/2, y));
		//if ( p % 2 == 0 ) { return J(x/2, y);}
		//if ( p % 2 == 1 ) { return -J(x/2, y);}
	} else {
		int p = (x-1) * (y-1) / 4;
		return (( p % 2 == 0) ? J(y % x, x) : -J(y % x, x));
		//if ( p % 2 == 0 ) { return J((y % x), x); }
		//if ( p % 2 == 1 ) { return -J((y % x), x); }
	}
}


int modular_equivalent(int b, int n)
{
	int x = (n - 1) / 2;
	int y = b;
	int a = 1;

	while ( x > 0) {
		if ( x % 2 == 1)
		{ a = powermod(a * y, 1, n); }
		y = powermod(y, 2, n);
		x = floor(x / 2);
	}

	return a;
}

int powermod(int base, int e, int mod)
{
	if ( e == 0 ) return 1;
	if ( e == 1 ) return base;

	int f = floor(e/2);
	int y = powermod(base, f, mod);
	y = (y * y) % mod;
	if ( e % 2 == 1 )
		y = (y * base) % mod;
	return y;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////




string random_string(const string& line)
{
	//line = "123123123123";
	string res = "";
	int length = line.size();

	// create a string with all 0s
	for (int i = 0; i < line.size(); i++)
	{
//		while ( (res[0] - '0') >= (line[0] - '0') )
//		{ res[0] = ((int) (8 * dshrandom(0) + 1) + '0')); }
		res.push_back((int) (0 + '0'));
	}
	
//	cout << " r_s().res = " << res << endl;
//	cout << " r_s().line = " << line << endl;

	for (int i = 0; i < length; i++)
	{
		//res[i] = ((int) (line[i] - '0') * dshrandom(0) + '0');
		res[i] = ((int) (9 * dshrandom(0) + 1) + '0');
	}
	//res[length-1] = ((int) (line[length-1] - '0' - 1 ) * dshrandom(0) + '0');

//	cout << " r_s().res = " << res << endl;
//	cout << " r_s().line = " << line << endl;

	return res;
}


static BigInt TWO = BigInt("2");
static BigInt ONE = BigInt("1");
static BigInt ZERO = BigInt("0");

bool is_probably_prime(BigInt& n)
{
	if ( BigInt::isEven(n) )
		return false;

	int test_count = 0;

	int k = 20;

	BigInt b;
	string l = random_string(n.to_string());

	while ( test_count < k ) {
		
		// get random big BigInt
		//b = (int) ((n - 2) * dshrandom(0) + 1);
		//BigInt b(random_string(l));
		b = BigInt(l);
		b.trimLeadingZeros(b);

		//cout << " pre.b = " << b << endl;
		while ( b >= n)
		{ b = b % n; }
		//cout << " after.b = " << b << endl;


		if ( GCD(b, n) != ONE )
		{ 
			//cout << "Good so far @ big int prime test GCD" << endl;
			return false; 
		}
		//else if ( J(b, n) == modular_equivalent(b, n) ) {
		//	cout << "Good so far @ big int prime test J & me" << test_count << endl;
		else if ( dshrandom(0) > 0.95 ) {
			test_count++;
		}

	}

	return true;

}


BigInt next_prime(BigInt& n)
{
	BigInt right = ( BigInt::isEven(n) ? (n + ONE) : (n + TWO));
	
	while ( !is_probably_prime(right) )
	{ right += TWO; }
	
	return right;
}

BigInt big_GCD(BigInt& x, BigInt& y)
{
	
	if ( x < y)
		return big_GCD(y,x);
	if ( y == ZERO)
		return x;
	if ( BigInt::isEven(x) )
	{
		if ( BigInt::isEven(y) )
		{
			BigInt a = (x / TWO);
			BigInt b = (y / TWO);
			BigInt c = big_GCD(a, b) * TWO;
			return c;
		}
		else
		{
			BigInt a = (x / TWO);
			return big_GCD(a, y);
		}
	} else {
		if ( BigInt::isEven(y) )
		{
			BigInt b = (y / TWO);
			return big_GCD(x, b);
		}
		else
		{
			BigInt b = (x - y);
			return big_GCD(y, b);
		}
	}
}


BigInt GCD(BigInt& x, BigInt& y)
{
	if ( y == ZERO ) {
		return x;
	} else if ( x < y) {
		return GCD(y, x);
	} 
	else {
		
		BigInt t = x % y;
		return GCD(y, t);
	}

}


// Jacobi Function
BigInt J(BigInt& x, BigInt& y)
{
	cout << "J.x = " << x << endl;
	cout << "J.y = " << y << endl;

	if ( x == ONE ) {
		return BigInt("1");
	} else if ( BigInt::isEven(x) ) {
		//BigInt tmp = BigInt(8);
		BigInt p = (y * y - BigInt("1")) / BigInt("8");
		BigInt r = x / BigInt("2");
		BigInt res = J(r, y);

		if ( BigInt::isEven(p) ) { return res;}
		else { return -res;}
	} else {
		BigInt xx = (x - BigInt("1"));
		BigInt yy = (y - BigInt("1"));
		BigInt p = (xx * yy) / BigInt("4");
		BigInt r = y % x;
		BigInt res = J(r, x);
		//return (p.even() ? res : res.set_sign(-1));
		if ( BigInt::isEven(p)) { return res;}
		else { return -res;}
	}
}

// modular equivalence
BigInt modular_equivalent(BigInt& b, BigInt& n)
{
	cout << "ME.b = " << b << endl;
	cout << "ME.n = " << n << endl;

	BigInt x = (n - BigInt("1")) / BigInt("2");
	BigInt y = b;
	BigInt a = BigInt("1");
	BigInt t = BigInt("2");

	while ( x > ZERO ) {
		if ( !BigInt::isEven(x) )
		{
			BigInt yy = a * y;
			a = yy % n;
		}
		y = (y * y) % n;
		x = x / BigInt("2");
	}

	cout << "ME().a = " << a << endl;

	return a;
}


BigInt powermod(BigInt& base, BigInt& e, BigInt& mod)
{
	if ( e == ZERO ) return BigInt(1);
	if ( e == ONE ) return base;

	BigInt f = e / TWO;
	BigInt y = powermod(base, f, mod);
	y = (y * y) % mod;
	if ( !BigInt::isEven(e) )
		y = (y * base) % mod;
	return y;
}
