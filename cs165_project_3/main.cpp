#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

extern "C" {
#include "utility.h"
};

using namespace std;


/**
 * Read a string from standard input stream and save to in.tmp
 */
void save_stdin_temp() {
	string line = "";
	cout << "Enter input string > ";
	getline(cin, line);

	ofstream tmp;
	tmp.open("in.tmp");

	if ( tmp.is_open() )
	{
		tmp << line;;
		tmp.close();
	}
	else
		cout << "Error: Unable to create tmp buffer" << endl;
}


int main(int argc, char* argv[])
{
	string line = "";
	string args[2];
	args[0] = "";
	args[1] = "";

	while ( 1 )
	{
		cout << "Enter command > ";
		getline(cin, line);
		//cout << line << endl;

		istringstream l(line);
		l >> args[0] >> args[1];

		//cout << "args[0] = " << args[0] << endl;
		//cout << "args[1] = " << args[1] << endl;

		if ( args[0] == "quit" )
		{
			break;
		}
		else if (( args[0] == "HUFF" || args[0] == "LZ1" || args[0] == "LZ2" ) && args[1] != "" )
		{
			string filename = args[1] + "_encode";
			encode(args[0].c_str(), args[1].c_str(), filename.c_str());
			args[1] = "";
		}
		else if (( args[0] == "HUFF" || args[0] == "LZ1" || args[0] == "LZ2" ) && args[1] == "")
		{
			save_stdin_temp();
			encode(args[0].c_str(), "in.tmp", "out_encode");
		}
		else if ( args[0] == "EXPAND" )
		{
			//cout << "Expanding file with " << args[1] << endl;
			string filename = args[1] + "_decode";
			expand(args[1].c_str(), filename.c_str());
			args[1] = "";
		}
		else if ( args[0] == "AUTO" )
		{
			//cout << "Doing benchmark test with test files" << endl;
			benchmark_test();
		}
		else
		{
			cout << "Error: unsupported command\n" << endl;
		}

	}

	return 0;
}