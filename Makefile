
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =

canny_par: canny_p.o main_p.cc timer.c
	g++ -o canny_par -I/data/apps/cuda/5.0/include -lpthread -L/data/apps/cuda/5.0/lib64 -lcudart -fopenmp -O3 -Wextra -g -lpng -I/data/apps/boost/1.57/include canny_p.o main_p.cc timer.c -I.

canny_p.o: canny_p.cu
	nvcc -c -arch=sm_20 canny_p.o canny_p.cu


canny_seq: main.cc timer.c canny_sequential.cc
	g++ -std=c++11 -O0 -g -lpng -I/data/apps/boost/1.57/include -o canny_seq main.cc timer.c canny_sequential.cc -I.


clean:
	rm -f canny_p.o canny_par

# eof
