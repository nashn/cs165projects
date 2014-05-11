#ifndef UTILITY_H
#define UTILITY_H


#include <cmath>
#include "BigInt.hpp"

extern "C" {
#include "dshrandom.h"
}

bool is_probably_prime(int n);

int closest_prime(int n);
int next_prime(int n);

// normal BigInt interval
int GCD(int x, int y);
int b_GCD(int x, int y);
unsigned int binary_GCD(unsigned int x, unsigned int y);

// Jacob Function
int J(int x, int y);

// b ^ [(N - 1) / 2] ( mod N)
// modular equivalence
int modular_equivalent(int b, int n);
// a function for powermod
int powermod(int base, int e, int mod);


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


bool is_probably_prime(BigInt& n);

BigInt next_prime(BigInt& n);

BigInt big_GCD(BigInt& x, BigInt& y);
BigInt GCD(BigInt& x, BigInt& y);

// Jacob Function
BigInt J(BigInt& x, BigInt& y);

// b ^ [(N - 1) / 2] ( mod N)
// modular equivalence
BigInt modular_equivalent(BigInt& b, BigInt& n);
// a function for powermod
BigInt powermod(BigInt& base, BigInt& e, BigInt& mod);



#endif