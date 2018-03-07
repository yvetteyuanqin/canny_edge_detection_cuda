
CC = gcc
CFLAGS = -std=c++11
COPTFLAGS = -O3 -g -lpng
LDFLAGS =


canny_seq: main.cc timer.c canny_sequential.cc 
	g++ -std=c++11 -O0 -g -lpng -I/data/apps/boost/1.57/include -o canny_seq main.cc timer.c canny_sequential.cc -I.

canny_par: main.cc timer.c canny_p.cc 
	g++ -std=c++11 -O0 -g -lpng -I/data/apps/boost/1.57/include -o canny_parallel main.cc timer.c canny_p.cc -I.





clean:
	rm -f

# eof
