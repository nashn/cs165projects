default:
	gcc -c -std=c99 huff.c -o huff.o
	gcc -c -std=c99 lz1.c -o lz1.o
	gcc -c -std=c99 lz2.c -o lz2.o
	gcc -c -std=c99 dshrandom.c -o dshrandom.o
	gcc -c -std=c99 timer.c -o timer.o
	gcc -c -std=c99 utility.c -o utility.o
	gcc -c -std=c99 expand.c -o expand.o
	g++ -c -std=c++0x main_huff.cpp -o main_huff.o
	g++ -c -std=c++0x main_lz1.cpp -o main_lz1.o
	g++ -c -std=c++0x main_lz2.cpp -o main_lz2.o
	g++ -c -std=c++0x main.cpp -o main.o
	g++ main_huff.o huff.o timer.o -o HUFF
	g++ main_lz1.o lz1.o timer.o -o LZ1
	g++ main_lz2.o lz2.o  timer.o -o LZ2
	gcc expand.o huff.o lz1.o lz2.o timer.o -o EXPAND
	g++ main.o utility.o timer.o dshrandom.o huff.o lz1.o lz2.o -o run

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf run
	rm -rf HUFF
	rm -rf LZ1
	rm -rf LZ2
	rm -rf EXPAND