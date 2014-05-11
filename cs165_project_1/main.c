// CS 165 Project 1
// Group Members
// Name: She Nie (UCInetID: nies)
// ID: 77134417
// Name: Chia-Hao Chen (UCInetID: nies)
// ID: 76213383


#include <stdio.h>
#include "utility.c"
#include "timer.c"


int main(int argc, char *argv[]) {
	
	// add more testcases here
	// rememeber to update the array size and fisrt loop times
	int n[4] = { 15, 100, 1000, 10000 };
	int k[4] = { 3, 10, 20, 40 };
	
	int test_time = 1000;
	//printf("Enter times for doing tests: ");
	//scanf("%d", &test_time);
	
	printf("********************** TEST START **********************\n");
	printf("--------------------------------------------------------\n");
	printf("n       k       Min.       Max.       Avg.       Runtime\n");
	printf("--------------------------------------------------------\n");
	// update the array size and fisrt loop times after adding new testcases 
	for (int i = 0; i < 4; i++) 
	{	
		int total = 0;
		int best = 1000000;
		int worst = 0;
		
		//printf("Test case for <%d, %d> pair:\n", n[i], k[i]);
		start_timer();
		// loop 1000 times with doalg(n, k)
		for(int j = 0; j < test_time; j++)
		{
			
			//int* res = doalg(n[i], k[i]);
			//int comparision = COMPARE( -1, k, res );
			int comparision = doalg(n[i], k[i]);
			
			//printf("The comparison # = %d\n", comparision);
			
			if (best > comparision)
			{
				best = comparision;
			}

			if (worst < comparision)
			{
				worst = comparision;
			}
			
			total += comparision;
		}

		printf("%6d  %6d  %6d  %6d      %.3f      %.3f\n", n[i], k[i], best, worst, (double)total/test_time, elapsed_time());
		//printf("The average case of element comparisons: %.3f\n", (double) total / test_time);
		//printf("The best case of element comparisons: %d\n", best);
		//printf("The worst case of element comparisons: %d\n\n", worst);
	}
	printf("--------------------------------------------------------\n");
	printf("The total runtime for whole program is: %.3f\n", user_time());
	printf("--------------------------------------------------------\n");
	printf("*********************** TEST END ***********************\n");
}
