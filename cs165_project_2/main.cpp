#include <iostream>
#include <string>

#include "utility.hpp"
#include "BigInt.hpp"


extern "C" {
#include "timer.h"
}

// CPP version of test code


using namespace std;

bool is_number(const string& line)
{
	for(int i = 0; i < line.size(); i++)
	{
		if ( !isdigit(line[i]) )
			return false;
	}
	return true;
}

void integer_primality_test(const string& line)
{
	//int a = to_number(line);
	int a = atoi(line.c_str());
	//BigInt a = parse_to_bigint(line);
	cout << "\nThe number:\n" << a << endl;

	start_timer();

	if ( is_probably_prime(a) )
	{
		printf("is prime. (%d digits,   %.2f secs)\n\n", (int)line.size(), user_time());
	} else {
		printf("is NOT prime. The next prime number is:\n");
		printf("%d", next_prime(a));//closest_prime(a));
		printf("\n (%d digits,   %.2f secs,   %.2f secs total))\n\n", (int)line.size(), elapsed_time(), user_time());
	}
}

void big_integer_primality_test(const string& line)
{
	//int a = to_number(line);
	//int a = atoi(line.c_str());
	BigInt a = BigInt(line);
	cout << "\nThe number:\n" << a << endl;

	start_timer();

	if ( is_probably_prime(a) )
	{
		cout << "is prime. (" << line.size() << " digits,   " << elapsed_time() << " secs)\n" << endl;
	} else {
		cout << "is NOT prime. The next prime number is:" << endl;
		BigInt next  = next_prime(a);
		cout << next ;//closest_prime(a));
		cout << "\n (" << line.size() << " digits,   " << elapsed_time() << " secs,   " << user_time() << " secs total))\n\n";;
	}
}

int main(int argc, char *argv[]) {

	string line = "";

	while (1) {
			
		cout << "Please enter a number terminated by a carriage return:" << endl;
	
		getline(cin, line);

		if ( line == "" || line == "0" )
		{
			break;
		}
		else if ( line == "1" )
		{
			cout << "is prime. (" << line.size() << " digits,   " << user_time() << " secs)\n" << endl;
		}
		else if ( !is_number(line) )
		{
			cout << "Invalid input.  Try agian.\n" << endl;
		}
		else
		{
			if ( line.size() > 10 )
			{ big_integer_primality_test(line); }
			else
			{ integer_primality_test(line); }
		}

	}

	cout << "Test End\n" << endl;

	return 0;
}
