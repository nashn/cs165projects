CS 165 Project 3
Group Members
-------------
Name: She Nie (UCInetID: nies)
ID: 77134417
Name: Greg Jeckell (UCInetID: gjeckell)
ID: 29774306


How to use the program
---------------------------
1.
To compile the program use
cd command in the terminal to change the current directory to where this project is located
2.
To build the project type
make 
in the terminal
3.
To run the program type
./run
in the terminal
4.
To clean up the program type
make clean
in the terminal


Data structure description and implementation
---------------------------------------------
The internal digits of our BigInt class are represented using the STL deque in reverse order (with higher order digits corresponding to a higher index). This data structure allows for fast (O(1)) insertion at the front and back of the collection, making shift operations easier to implement than the similar vector class. 

Theoretical time  analysis
---------------------------
The theoretical run times of the following functions are approximately as follows:
GCD ==> O(log(n))
J        ==> O(log(n))
modular_exponential ==> O(log(n))

However, the time analysis is greatly limited by the individual arithmetic operations of the BigInt class. The following approximate run times of these operations are as follows:

Addition/Subtraction ==> O(n)
Multiplication            ==> O(n^2)
KaratsubaMultplication ==> O(n^(1.59))
Long Division ==> O(quotient * n)

Primality probability k factor:
Since we want to have a probability of less than 1/1000000 to get the test result wrong, we evaluate k times at 1/2 probability each, yielding the following equation to get the k factor: 1/2^k < 1/1000000. Solving for k yields approximately 20.


Result
--------

Based on your empirical observations (and extrapolations), using 20 iterations in the main loop, how much time would your system require to determine whether

a 100 decimal digit number is prime (when it actually is prime)?
- Approximately 90 seconds on average


a 1000 decimal digit number is prime?
- Approximately 850 seconds on average


a 10000 decimal digit number is prime?
- Theoretically 810,000 seconds 



Based on your observations, how many iterations of the main loop should be performed to enable stating that a number is prime with probability of error less than one in a trillion?

By solving the equation of 1/2^k < 1/ 1,000,000,000,000, we can get the probability factor k = log(2, 10^12) = 39 == 40 (times)


Space Analysis:
-------------------
Our BigInt implementation uses base 10 for storing each digit resulting in O(n) integers for a single representation of a BigInt.