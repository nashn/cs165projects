// main_huff.c

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <iostream>
#include <string>
#include <fstream>

extern "C" {
#include "timer.h"
#include "huff.h"
}

using namespace std;

int get_file_size(const char* filename)
{
	int fsize = 0;

	FILE* f = fopen(filename, "r+b");
	// seek to end of file
	fseek(f, 0, SEEK_END);
	// seek to end of file
	fsize = ftell(f);
	// get current file pointer
	fseek(f, 0, SEEK_SET);

	fclose(f);

	return fsize;
}

/*
void save_stdin_to_file()
{
	char content[1000];
	FILE* f;
	printf("Enter input string > ");
	scanf("%s", content);
	printf("The input string is %s\n", content);
	f = fopen("in.tmp", "w"); //
	fputs(content, f);
	fclose(f);
}
*/
void save_stdin_to_file()
{
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

int main(int argc, const char* argv[])
{

	FILE* infile;
	FILE* outfile;

	if ( argc == 1 )
	{
		save_stdin_to_file();
		infile = fopen("in.tmp", "r+b");
		string outfilename = "outfile_encode";
		outfile = fopen(outfilename.c_str(), "w+b");

		int fsize = get_file_size("in.tmp");
		printf("standard input     \n");
		printf("size         %d\n", fsize);
		printf("----\n");
		start_timer();
		huff(infile, outfile);

		int COMPsize = get_file_size("outfile_encode");
		printf("HUFFsiz      %d\n", COMPsize);
		printf("%%save        %.2f%%\n", (1 - (double)COMPsize / (double)fsize) * 100);
		printf("t-cmprss     %.2f\n", elapsed_time());
	}
	else if ( argc == 2 )
	{
		infile = fopen(argv[1], "r+b");
		string outfilename = "outfile_encode";
		outfile = fopen(outfilename.c_str(), "w+b");

		int fsize = get_file_size(argv[1]);
		printf("filename     %s\n", argv[1]);
		printf("size         %d\n", fsize);
		printf("----\n");
		start_timer();
		huff(infile, outfile);

		int COMPsize = get_file_size("outfile_encode");
		printf("HUFFsiz      %d\n", COMPsize);
		printf("%%save        %.2f%%\n", (1 - (double)COMPsize / (double)fsize) * 100);
		printf("t-cmprss     %.2f\n", elapsed_time());
	}
	else
		printf("Usage: HUFF or HUFF <filname>\n");

	return 0;
}