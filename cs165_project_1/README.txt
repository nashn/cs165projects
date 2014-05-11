CS 165 Project 1
Group Members
-------------
	Name: She Nie (UCInetID: nies)
	ID: 77134417

	Name: Chia-Hao Chen (UCInetID: nies)
	ID: 76213383


How to use the program
---------------------------
1.
To compile the program, use 
	cd ~/current working directory
command in the terminal to change the current directory to where this project is located
2.
To build the project, type in
	make 
in the terminal
3.
To run the program, type in
	./run
in the terminal
if want to save the output, use
	./run >> OUTPUT.txt
instead
4.
To clean up the program, type
	make clean
in the terminal

Data structure description and implementation
---------------------------------------------

For this project we used a combination of "heap sort", "merge sort" and "array based minHeap"
First we sort the first k element with HeapSort and store it into heapArray(heapArray is maintained by
using MinHeap data structure properties), then for the rest of the elements in the array, we compare them with 
the root of the heapArray, if the element is greater than the root then we remove the root and add the 
element into the heapArray.

HeapSort:
	Sort the index array, using array based minHeap to do the sorting

MergeSort:
	Using divide-and-conqur techniques here. Dynamically allocating subarray to minimize the comparision

Array-Based MinHeap:
	we used "moveDown", "siftUp", "insert", "removeMin" functions to build up our minHeap
	where the index of a parent is (NodeIndex - 1)/2 , index for leftchild 2*NodeIndex + 1, 
	index for rightchild 2*NodeIndex +2
	
Note: 
1. To handle some special cases (when k >= n / 2 ), we implement a subroutine to use simple MergeSort to
	deal with these cases, in order to reduce comparisions.

Space Analysis
--------------
index array          n
heap array           k
res array            k + 1
MergeSort            n

Total space          2n + 2k + 1


Theorectical time  analysis
---------------------------

To sort the first k element using HeapSort takes                          k log k
To compare the rest of the elements with the minimum of this array takes (n-k) log k
To sort the final heap array using mergeSort takes                        k log k
Total theoretical upper bound t                                          (n+k) log k

Testcases Data:
********************** TEST START **********************
--------------------------------------------------------
n       k       Min.       Max.       Avg.       Runtime
--------------------------------------------------------
    15       3      17      47      30.203      0.003
   100      10     213     376      299.993      0.015
  1000      20    1617    2051      1823.253      0.095
 10000      40   12297   13194      12765.386      0.811
--------------------------------------------------------
The total runtime for whole program is: 0.926
--------------------------------------------------------
*********************** TEST END ***********************

For <15, 3> case, Theoretical (15 + 3) log 3 = 28.5
For <100, 10> case, Theoretical (100 + 10) log 10 =  365.4
For <1000, 20> case, Theoretical (1000 + 20) log 20 = 4408.3
For <10000, 40> case, Theoretical (10000 + 40) log 40 = 53432.15


Based on our result the worst case for n < 100 is worst than theoretical input because our idea is based on
first selecting k element, then rest first of the elements will be compared with the smallest of those k, since with n < 100 
our chances of picking small number is large and therefor we will have to do more comparision.
With n > 100 changes of selecting small element becomes smaller hence our performance imporved.
