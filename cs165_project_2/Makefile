default:
	gcc -c -std=c99 dshrandom.c -o dshrandom.o
	gcc -c -std=c99 timer.c -o timer.o
	g++ -c -std=c++0x utility.cpp -o utility.o
	g++ -c -std=c++0x BigInt.cpp -o BigInt.o
	g++ -c -std=c++0x main.cpp -o main.o
	g++ main.o utility.o BigInt.o dshrandom.o timer.o -o run

clean:
	rm -rf *.o
	rm -rf run
