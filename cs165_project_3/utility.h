// utility.h
#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "dshrandom.h"
#include "timer.h"

#include "huff.h"
#include "lz1.h"
#include "lz2.h"

// Global file pointers
FILE* infile;
FILE* outfile;

// encode function calls
void encode(const char* type, const char* infilename, const char* outfilename);

// decode function calls
void expand(const char* infilename, const char* outfilename);

// functions for auto testing
int get_file_size(const char* filename);
void benchmark_test();


#endif