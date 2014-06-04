// utility.c

#include "utility.h"

// helper functions;

// encode function calls
void encode(const char* type, const char* infilename, const char* outfilename)
{
	infile = fopen(infilename, "r+b");
	outfile = fopen(outfilename, "w+b");

	printf("filename     %s\n", infilename);
	//printf("size         %d\n", get_file_size(infilename));
	printf("----\n");
	if ( strcmp(type, "HUFF") == 0 )
	{
		start_timer();
		huff(infile, outfile);
		printf("t-cmprss     %.2f\n", elapsed_time());
	}
	else if ( strcmp(type, "LZ1") == 0 )
	{
		start_timer();
		lz1(infile, outfile);
		printf("t-cmprss     %.2f\n", elapsed_time());
	}
	else if ( strcmp(type, "LZ2") == 0 )
	{
		start_timer();
		lz2(infile, outfile);
		printf("t-cmprss     %.2f\n", elapsed_time());
	}
	else
		printf("Error: unknown encode function call");
}


// decode function calls
void expand(const char* infilename, const char* outfilename) {
	infile = fopen(infilename, "r+b");
	outfile = fopen(outfilename, "w+b");

	// check magic number
	char head;
	fseek(infile, 0L, SEEK_SET);
	head = getc(infile);

	if (head == 13) {
		unhuff(infile, outfile);
	}
	if (head == 17) {
		unlz1(infile, outfile);
	}
	if (head == 19) {
		unlz2(infile, outfile);
	}
}


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
void benchmark_test()
{
        printf("Hello World!\n");
}
*/

void benchmark_test()
{
	char* benchmark_set[3] = {"news", "book1", "kennedy.xls"};
	char* benchmark_encode[3] = {"news_encode", "book1_encode", "kennedy.xls_encode"};
	char* benchmark_decode[3] = {"news_decode", "book1_decode", "kennedy.xls_decode"};
	char* command[3] = {"gzip news", "gzip book1", "gzip kennedy.xls"};
	char* command_encode[3] = {"news.gz", "book1.gz", "kennedy.xls.gz"};
	char* command_decode[3] = {"gzip -d news.gz", "gzip -d book1.gz", "gzip -d kennedy.xls.gz"};

	for (int i = 0; i < 3; i++)
	{

		int fsize = get_file_size(benchmark_set[i]);
		int COMPsize = 0;
		printf("filename     %s\n", benchmark_set[i]);
		printf("size         %d\n", fsize);
		printf("----\n");

		///////////////////////////////////////////////
		start_timer();
		infile = fopen(benchmark_set[i], "r+b");
		outfile = fopen(benchmark_encode[i], "w+b");
		huff(infile, outfile);
		COMPsize = get_file_size(benchmark_encode[i]);

		printf("HUFFsiz      %d\n", COMPsize);
		printf("%%save        %.2f%%\n", (1 - (double)COMPsize / (double)fsize) * 100);
		printf("t-cmprss     %.2f\n", elapsed_time());

		start_timer();
		infile = fopen(benchmark_encode[i], "r+b");
		outfile = fopen(benchmark_decode[i], "w+b");
		huff(infile, outfile);
		printf("t-expand     %.2f\n", elapsed_time());
		////////////////////////////////////////////////

		printf("----\n");

		start_timer();
		infile = fopen(benchmark_set[i], "r+b");
		outfile = fopen(benchmark_encode[i], "w+b");
		lz1(infile, outfile);
		COMPsize = get_file_size(benchmark_encode[i]);

		printf("LZ1siz       %d\n", COMPsize);
		printf("%%save        %.2f%%\n", (1 - (double)COMPsize / (double)fsize) * 100);
		printf("t-cmprss     %.2f\n", elapsed_time());

		start_timer();
		infile = fopen(benchmark_encode[i], "r+b");
		outfile = fopen(benchmark_decode[i], "w+b");
		unlz1(infile, outfile);
		printf("t-expand     %.2f\n", elapsed_time());

		////////////////////////////////////////////////

		printf("----\n");
		start_timer();
		infile = fopen(benchmark_set[i], "r+b");
		outfile = fopen(benchmark_encode[i], "w+b");
		lz2(infile, outfile);
		COMPsize = get_file_size(benchmark_encode[i]);

		printf("LZ2siz       %d\n", COMPsize);
		printf("%%save        %.2f%%\n", (1 - (double)COMPsize / (double)fsize) * 100);
		printf("t-cmprss     %.2f\n", elapsed_time());

		start_timer();
		infile = fopen(benchmark_encode[i], "r+b");
		outfile = fopen(benchmark_decode[i], "w+b");
		unlz2(infile, outfile);
		printf("t-expand     %.2f\n", elapsed_time());
		////////////////////////////////////////////////

		printf("----\n");

		start_timer();
		system(command[i]);
		COMPsize = get_file_size(command_encode[i]);

		printf("GZIPsiz      %d\n", COMPsize);
		printf("%%save        %.2f%%\n", (1 - (double)COMPsize / (double)fsize) * 100);
		printf("t-cmprss     %.2f\n", elapsed_time());

		start_timer();
		system(command_decode[i]);
		printf("t-expand     %.2f\n", elapsed_time());
		////////////////////////////////////////////////
		printf("\n\n\n");
	}
}
