// CS 165 Project 1
// Group Members
// Name: She Nie (UCInetID: nies)
// ID: 77134417
// Name: Chia-Hao Chen (UCInetID: nies)
// ID: 76213383


#ifndef UTILITY_H
#define UTILITY_H


// helper functions for all the below functions
void swap(int* l, int a, int b);


// decision tree algorthim for sorting 3 items
int sort3(int x, int y, int z);

// selection algorithm for get good initial result
int select3(int* indices, int* check_map, int start, int end);
int selectMedian(int* arr, int* check_map, int size, int position);

// heap-utilities
void moveUp(int* heap, int position);
void moveDown(int*heap, int position, int size);
void siftDown(int* heap, int root, int end);

void removeMin(int* heap, int size) ;
void insert(int* heap, int value, int position);

// function signatures for heapSort()
//void moveDown(int* heap, int root, int bottom);
void heapSort(int* heap, int size);



// functions for doing mergeSort
int* mergeSort(int* x, int n);
int* merge(int* left, int ln, int* right, int rn);

// special case handler
int doalg_nk(int* indices, int n, int k);


#endif