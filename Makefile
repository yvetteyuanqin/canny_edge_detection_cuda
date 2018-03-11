
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =

canny_par: canny_p.o main_p.cc timer.c
    g++ -O3 -g -lpng -I/data/apps/boost/1.57/include -o canny_par canny_p.o main_p.cc timer.c -I.

canny_p.o: canny_p.cu
#nvcc -O3 -g  -o -arch=sm_20 canny_p.o timer.c canny_p.cu -I.
    nvcc -c -arch=sm_20 canny_p.o canny_p.cu


canny_seq: main.cc timer.c canny_sequential.cc 
	g++ -std=c++11 -O0 -g -lpng -I/data/apps/boost/1.57/include -o canny_seq main.cc timer.c canny_sequential.cc -I.


clean:
	rm -f

# eof
