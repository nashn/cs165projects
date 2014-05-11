// CS 165 Project 1
// Group Members
// Name: She Nie (UCInetID: nies)
// ID: 77134417
// Name: Chia-Hao Chen (UCInetID: nies)
// ID: 76213383


#include <stdlib.h>
#include "COMPARE.c"
#include "utility.h"



int doalg(int n, int k){

	// initialize root node & compare arry
	COMPARE(0, n);
	// initilize the indices array
	int indices[n];

	for (int i = 0; i < n; i++) {
		indices[i] = i+1;
	}
	
	// special case
	if ( k >= n/2 ) {
		return doalg_nk(indices, n, k);
	}


	int heap[k];
	
/*
	// random select k elements
	// not quick work out
	//////////////////////////////////////////////////////
	int check_map[n];
	for (int i = 0; i < n; i++)
	{ check_map[i] = 0; }

	for (int i = 0; i < k; i++) {
		heap[i] = selectMedian(indices, check_map, n, n/2);
		//heap[i] = select3(indices, check_map, i, n-i);
	}

	//////////////////////////////////////////////////////*/
	
	
	// select first k elements
	for (int i = 0; i < k; i++) {
		heap[i] = indices[i];
	}
	
	heapSort(heap, k);

	// need to work out insert() to replace the heapSort() inside this loop
	// && CHECK(i) != 1
	for (int i = k; i < n; i++) {
		if ( COMPARE(heap[0], indices[i]) == 2 ) {
			removeMin(heap, k);
			insert(heap, indices[i], k);
		}
	}
	
	/*
	// the following part is for random selection
	heapSort(heap, k);

	// && CHECK(i) != 1
	for (int i = 0; i < n; i++) {
		if ( check_map[i] != 1 && COMPARE(heap[0], indices[i]) == 2 ) {
			removeMin(heap, k);
			insert(heap, indices[i], k);
		}
	}
	//////////////////*/

	// using better sorting algorithm here
	int* best_res = mergeSort(heap, k);

	// read the last k elements in reverse order
	int res[k+1];
	for (int i = 1; i <= k; i++) {
		res[i] = best_res[k-i];
		//printf("i=%d    heap[%d]=%d    res[%d]=%d \n", i, n-i, heap[k-i], i, res[i]);
	}
	
	return (COMPARE( -1, k, res ));
}

// special case handler
int doalg_nk(int* indices, int n, int k) {
	int* partial_res = mergeSort(indices, n);

	int res[k+1];
	for (int i = 1; i <= k; i++) {
		res[i] = partial_res[n-i];
	}

	return COMPARE( -1, k, res );
}

/*
** Swap elements in index a and index b in array l
*/
void swap(int* l, int a, int b)
{
	int temp = l[a];
	l[a] = l[b];
	l[b] = temp;
}

// decision tree algorthim for sorting 3 items
int sort3(int x, int y, int z) {
	if ( COMPARE(x, y) == 2 ) {
		if ( COMPARE(y, z) == 2 ) {
			return y;
		} else if ( COMPARE(x, z) == 2 ) {
			return z;
		} else { return x; }
	} else if ( COMPARE(z, y) == 2 ) {
			return y;
	} else if ( COMPARE(x, z) == 2 ) {
			return x;
	} else { return z; }		
}


// selection algorithm for get good initial result
int select3(int* indices, int* check_map, int start, int end) {
	if (start == end) {
		return indices[start];
	}

	int lpivot = indices[start];
	int pivot =  (indices[start] + indices[end]) / 2;//(int)end * dshrandom(0);
	int rpivot = indices[end];

	while ( pivot == 0 || check_map[pivot] == 1 ) {
		pivot = (start + end) / 2;//(int)end * dshrandom(0);
		if ( check_map[lpivot] == 1 ) {
			lpivot++;
		}
		else if ( check_map[rpivot] == 1 ) {
			rpivot++;
		}
	}

	pivot = sort3(lpivot, pivot, rpivot);
	check_map[pivot] = 1;

	return indices[pivot];
}

int selectMedian(int* arr, int* check_map, int size, int position) {
	int pivot = size * dshrandom(0);
	while ( pivot == 0 || check_map[pivot] == 1 ) {
		pivot = size * dshrandom(0);
	}

	check_map[pivot] = 1;
	
	if ( COMPARE(arr[position], arr[pivot]) == 1 ) {
		return arr[pivot]--;
	} else {
		return arr[pivot]++;
	}
}



/* moveUp moves the newly inserted value to its
** correct location in the heap array
** INPUT: heap array, index of the node to be shifted up
** OUTPUT: None
*/
void moveUp(int* heap, int position)
{
	if(position !=0){
		int parent = (position - 1) / 2;
		if ( COMPARE(heap[parent], heap[position]) == 1 ) {
			swap(heap, parent, position);
			moveUp(heap, parent);
		}
	}
}


/* moveDown function is used to find the correct position
** for the element that was moved up to the root after removeMin()
** is called.
** INPUT: heap array, index of the data to be moved, size of the current heap
** OUTPUT: None
*/
void moveDown(int* heap, int position, int size) {

	int min = 0;
	
	int left = 2 * position + 1;
	int right = 2 * position + 2;
	if (right >= size) {
		if (left >= size)
			return;
		else
			min = left;
	} else {
		min = (( COMPARE(heap[left], heap[right]) == 2 ) ? left : right);
	}

	if ( COMPARE(heap[position], heap[min]) == 1 ) {
		swap(heap, min, position);
		moveDown(heap, min, size);
	}
}

/* removeMin removes the minimum number
** in the heap array
** INPUT: heap array, size of the current heap
** OUTPUT: None
*/
void removeMin(int* heap, int size) {
	heap[0] = heap[size - 1];
	moveDown(heap, 0, size);
}


/* insert: insert the number into currect position
** in the heap array
** INPUT: heap array, value to be added, size of the current heap
** OUTPUT: None
*/
void insert (int* numbers, int value, int position)
{
	numbers[position-1] = value;
	moveUp(numbers, position-1);
}


/* siftDown is used to relocate a element in the heap array
** INPUT: heap array, root of the heap, last element of the heap
** OUTPUT: None
*/
void siftDown(int* heap, int root, int end) {
	int max = root * 2 + 1;

	// Find the biggest child
	if( max < end ) {
		int other = max + 1;
		// Reversed for stability
		max = (( COMPARE(heap[other], heap[max]) == 1 ) ? other : max);
	} else {
		// Don't overflow
		if(max > end) return;
	}

	// If we have the correct ordering, we are done.
	if( COMPARE(heap[root], heap[max]) == 1 ) return;

	// Swap
	swap(heap, root, max);

	// Tail queue recursion. Will be compiled as a loop with correct compiler switches.
	siftDown(heap, max, end);
}

/*
** heapSort sorts an array of size array_size
** INPUT: heap array, array size
*/
void heapSort(int* heap, int size) {
	for (int i = (size / 2); i >= 0; i--) {
		siftDown(heap, i, size - 1);
		//moveDown(heap, i, size - 1);
	}

	for (int i = size - 1; i >= 1; i--) {
		swap(heap, 0, i);
		siftDown(heap, 0, i - 1);
		//moveDown(heap, 0, i-1);
	}
}


/*
** mergeSort is called when heap array is nearly sorted
** modified from cs161 notes
*/
int* mergeSort(int* x, int n)
{
	if (n < 2) {
		return x;
	}

	int half = (int) n / 2;
	int rhalf = n - half;

	int* left = (int *) malloc(sizeof(int) * half);
	//int left[half];
	for (int i = 0; i < half; i++)
	{ left[i] = x[i]; }

	int* right = (int *) malloc(sizeof(int) * rhalf);
	//int right[rhalf];
	for (int i = 0; i < rhalf; i++)
	{ right[i] = x[half+i]; }

	left = mergeSort(left, half);
	right = mergeSort(right, rhalf);

	return merge(left, half, right, rhalf);
}

/*
** Helper function for mergeSort
** modified from cs161 notes
*/
int* merge(int* left, int ln, int* right, int rn)
{
	int* res = (int *) malloc(sizeof(int) * (ln + rn));
	int i = 0;
	int j = 0;

	while (i < ln && j < rn) {
		if ( COMPARE(left[i], right[j]) == 2 ) {
			res[i+j] = left[i];
			i++;
		}
		else {
			res[i+j] = right[j];
			j++;
		}
	}
	while ( i < ln ) {
		res[i+j] = left[i];
		i++;
	}
	while ( j < rn ) {
		res[i+j] = right[j];
		j++;
	}
	return res;
}

