CS 165 Project 3
Group Members
-------------
Name: She Nie (UCInetID: nies)
ID: 77134417
Name: Xv Gao (UCInetID: xug2)
ID: 70119773


How to compile and use the program
----------------------------------
1. To compile the program use "cd" command in the terminal to change the current directory to where this project is located

2. To build the project type
		make 
in the terminal

3. To run the program, follow below steps
IMPORTANT:
	Before running any of the following steps, please COPY and PASTE all the 
	test files to the same directory as the executable files. 

There are two different modes to turn tests:
	1)  Using ./run to get into a command line inter face, where all the 
		routines (HUFF, LZ1, LZ2, EXPAND) are treated as commands. There are 
		also a command AUTO to run 4 algorithms through all the test files
	2)  Seperated routines can also be executed by using ./HUFF <filename>
		to apply Huffman encoding to this file. The similar commands also 
		apply to the other routines (./LZ1, ./LZ2 and ./EXPAND)

4. To clean up the program type
		make clean
in the terminal


Data structure description and implementation
---------------------------------------------
1) Huffman
We implement structures as the node for Huffman encoding trees. To do the encription, we need to read the input file twice as described in the lecture notes. The fisrt time reading through the file will build up the frequency table, which will be a size of 256 integer array, since there can be only 256 different characters. After we have the frequency table, it will build up the Huffman tree structure base on that, whcih takes k log k steps. At the second time, the program will use bit operations to "walk" through the Huffman tree to get the correspoding codes of the input text.

2) Lempel-Ziv Variation 1
LZ1, also known as LZ77, which has a window size of 2048, has a maximum string length is 16. We implement this algorithm by using an array based tree structure. The program will look at the 16-size buffer to do a pattern matching search in the 2048-size tree structure. 

3) Lempel-Ziv Variation 2
LZ2, also known as LZ78, which has a window size of 4096, has a maximum string length is 16. The implementation of this algorithm is pretty much the same as the LZ77, but the differences exist in doing the pattern matching search through the tree structure. 

Note: For the LZ1 and LZ2, we did not have enougth time to implement a hash table structure to store all the patterns. Base on our research online, the algorithm will be faster than our current results, if we implement a randomize hashing structure.

4) Interface
We implement two different interfaces since we got a little clarification from the professor. The first interface is a command line tool that supports five different commands: HUFF, LZ1, LZ2, EXPAND, AUTO (an auto testing command). The second interface is four seperated executable files, which can be executed by invoking "./HUFF <filename>" with an optional filname parameter. With the filename, it would apply the specific algorithm to the file; without the filename, it would prompt the user for a standard input string to do the compression. 



Benchmark Results
-----------------
filename     news           book1           kennedy.xls
size         377109			768771			1029744
----					-- blank row --
HUFFsiz      247423			439403			463561
%save        34.39%			42.84%			54.98%
t-cmprss     0.09			0.03			0.15
t-expand     0.05			0.00			0.05
----					-- blank row --
LZ1siz       219766			475975			287846
%save        41.72%			38.08%			72.05%
t-cmprss     0.12			0.27			3.33
t-expand     0.01			0.02			0.02
----					-- blank row --
LZ2siz       195893			424252			288124
%save        48.05%			44.81%			72.02%
t-cmprss     0.17			0.31			4.71
t-expand     0.01			0.05			0.02
----					-- blank row --
GZIPsiz      144840			313375			206778
%save        61.59%			59.24%			79.92%
t-cmprss     0.00			0.00			0.00
t-expand     0.00			0.00			0.00
-----------------
Note: The above data is collected from the executions in Openlab computer.



Result Analysis
---------------
Based on the research data online, we know that there are various efficient data compression algorithms for different types of data. As presented above, our implementation of shows the efficiency of the three different algorithms.


Theoretical time analysis
--------------------------
1) Huffman Runtime
Note: k = different characters' size = 256
Encode:
To build up frequency table (size = k) for input n characters           O(n)
To construct the Huffman tree in array representation for k characters  O(k log k)
To walk through the input n characters to build up Huffman code         O(n)
Total theoretical time                                                  O(2n + k log k)

Decode:
To load the first k encoded Huffman tree node characters                k
To construct the Huffman tree in array representation for k characters  k log k 
To walk through the input n-k characters to recover original data       (n-k)
Total theoretical time                                                  (n + k log k)


2) Lempel-Ziv Variation 1 Runtime
initLz1Tree()   : O(log n)
insertLz1Node() : O(log n)
deleteLz1Node() : O(log n)
lz1()           : O(n)
unlz1()         : O(n)

Encode:
Total theoretical time                                                 O(n log n)

Decode:
Total theoretical time                                                 O(n log n)


3) Lempel-Ziv Variation 2 Runtime

initLz2Tree()   : O(log n)
insertLz2Node() : O(log n)
deleteLz2Node() : O(log n)
lz2()           : O(n)
unlz2()         : O(n)

Encode:
Total theoretical time                                                 O(n log n)

Decode:
Total theoretical time                                                 O(n log n)


4) System Utility (gzip)
Expectedly, the system utility program, gzip, achieves the best compression rates and shorest compression time for three benchmark files. By researching online, we know that the gzip is actually implementing the same algorithm as our requirement, the LZ77 algorithm, with better optimizations.


Space Analysis
--------------
1) Huffman Tree
Each Huffman tree node can be expected to be represented by 1 bits, for the tree structure, we need at least k log k + 8k bits to store all the Huffman tree structure. To store all the encode bits, based on the imformation theory, we know that each character can be potentially expected to redcuce 4 bits. The space for storing bits of n characters will be about 5 * n, after compression.
In sum, it requires O(k^2 + n log n) space to do both compression and decompression processes.

2) Lempel-Ziv Variation 1 and 2
Variation 1 requires a 2048 bytes array to do all the encoding and decoding processes.
Variation 2 requires a 4096 bytes array to do all the encoding and decoding processes.
