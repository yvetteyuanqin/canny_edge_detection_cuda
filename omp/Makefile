
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =



canny_omp: main.cc timer.c canny_omp.cc
	g++ -std=c++11 -O3 -g -fopenmp -lpng -I/data/apps/boost/1.57/include -o canny_omp main.cc timer.c canny_omp.cc -I.


clean:
	rm -f canny_omp.o canny_omp

# eof
