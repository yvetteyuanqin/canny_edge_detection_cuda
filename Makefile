
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =

canny_par: canny_parallel.o main_p.cc timer.c
    g++ -std=c++11 -O3 -g -lpng -I/data/apps/boost/1.57/include -o canny_par canny_parallel.o main_p.cc timer.c -I.

canny_parallel.o: timer.c canny_p.cu
	nvcc -arch compute_50 -O3 -g  -o canny_parallel.o timer.c canny_p.cu -I.



canny_seq: main.cc timer.c canny_sequential.cc 
	g++ -std=c++11 -O0 -g -lpng -I/data/apps/boost/1.57/include -o canny_seq main.cc timer.c canny_sequential.cc -I.


clean:
	rm -f

# eof
