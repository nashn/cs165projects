// expand.c

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "timer.h"

#include "huff.h"
#include "lz1.h"
#include "lz2.h"

FILE* infile;
FILE* outfile;

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

void expand(const char* infilename, const char* outfilename) {
	infile = fopen(infilename, "r+b");
	outfile = fopen(outfilename, "w+b");

	printf("filename     %s\n", infilename);
	printf("Expand to    %s\n", outfilename);
	printf("----\n");

	// check magic number
	char head;
	fseek(infile, 0L, SEEK_SET);
	head = getc(infile);

	int COMPsize = 0;

	if (head == 13) {
		start_timer();
		unhuff(infile, outfile);
		COMPsize = get_file_size(outfilename);
		printf("EXPANDsiz      %d\n", COMPsize);
		printf("t-expand     %.2f\n", elapsed_time());
	}
	if (head == 17) {
		start_timer();
		unlz1(infile, outfile);
		COMPsize = get_file_size(outfilename);
		printf("EXPANDsiz      %d\n", COMPsize);
		printf("t-expand     %.2f\n", elapsed_time());
	}
	if (head == 19) {
		start_timer();
		unlz2(infile, outfile);
		COMPsize = get_file_size(outfilename);
		printf("EXPANDsiz      %d\n", COMPsize);
		printf("t-expand     %.2f\n", elapsed_time());
	}
}

int main(int argc, const char* argv[])
{

	if ( argc == 2 )
	{
		char* outfilename = "outfile_decode";
		expand(argv[1], outfilename);
	}
	else
		printf("Usage: EXPAND <filname>\n");

	return 0;
}